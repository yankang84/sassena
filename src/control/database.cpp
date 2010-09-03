// direct header
#include "control/database.hpp"

// standard header
#include <cmath>
#include <fstream>
#include <iostream>
#include <vector>
#include <string>

// special library headers
#include <boost/filesystem.hpp>
#include <boost/math/special_functions/factorials.hpp>
#include <boost/regex.hpp>

// other headers
#include "log/log.hpp"
#include "control/parameters.hpp"
#include "io/xml_interface.hpp"

using namespace std;

void Database::read_xml(std::string filename) {
		
	
	// store the database line by line into an internal buffer, 
	// this is for keeping history
	ifstream dbfile(filename.c_str());
	string line;
	while (getline(dbfile,line)) {
		carboncopy.push_back(line);
	}
	dbfile.close();
	
	XMLInterface xmli(filename);

    Info::Inst()->write("Reading database...");
	// now read the database
	
	// START OF database section //
	if (xmli.exists("//names/pdb")) {
		vector<XMLElement> elements = xmli.get("//names/pdb/element");
		for(size_t i = 0; i < elements.size(); ++i)
		{
			xmli.set_current(elements[i]);
			string label = xmli.get_value<string>("./name");
			string regexp = xmli.get_value<string>("./param");
			names.pdb.reg(label,regexp);
		}
	}

	if (xmli.exists("//masses")) {
		vector<XMLElement> elements = xmli.get("//masses/element");
		for(size_t i = 0; i < elements.size(); ++i)
		{
			xmli.set_current(elements[i]);
			string label = xmli.get_value<string>("./name");
			double mass = xmli.get_value<double>("./param");
			size_t ID = atomIDs.get(label);
			masses.reg(ID,mass);
		}
	}


	if (xmli.exists("//sizes")) {
		vector<XMLElement> elements = xmli.get("//sizes/element");
		for(size_t i = 0; i < elements.size(); ++i)
		{
			xmli.set_current(elements[i]);
			string label = xmli.get_value<string>("./name");
			size_t sizes_function_type = xmli.get_value<size_t>("./type");
			
			vector<double> values;
			vector<XMLElement> params = xmli.get("./param");
			for(size_t i = 0; i < params.size(); ++i)
			{
				xmli.set_current(params[i]);
				values.push_back(xmli.get_value<double>("."));
			}
			
			size_t ID = atomIDs.get(label);
			volumes.reg(ID,values,sizes_function_type);
		}
	}
	
	
	if (xmli.exists("//exclusionfactors")) {
		vector<XMLElement> elements = xmli.get("//exclusionfactors/element");
		for(size_t i = 0; i < elements.size(); ++i)
		{
			xmli.set_current(elements[i]);
			string label = xmli.get_value<string>("./name");
			size_t sizes_function_type = xmli.get_value<size_t>("./type");
			
			vector<double> values;
			vector<XMLElement> params = xmli.get("./param");
			for(size_t i = 0; i < params.size(); ++i)
			{
				xmli.set_current(params[i]);
				values.push_back(xmli.get_value<double>("."));
			}
			
			size_t ID = atomIDs.get(label);
			exclusionfactors.reg(ID,values,sizes_function_type);
		}
	}


	if (xmli.exists("//scatterfactors")) {
		vector<XMLElement> elements = xmli.get("//scatterfactors/element");
		for(size_t i = 0; i < elements.size(); ++i)
		{
			xmli.set_current(elements[i]);
			string label = xmli.get_value<string>("./name");
			size_t factors_function_type = xmli.get_value<size_t>("./type");
			
			vector<double> values;
			vector<XMLElement> params = xmli.get("./param");
			for(size_t i = 0; i < params.size(); ++i)
			{
				xmli.set_current(params[i]);
				values.push_back(xmli.get_value<double>("."));
			}
			
			size_t ID = atomIDs.get(label);
			sfactors.reg(ID,values,factors_function_type);
		}
	} else {
		Err::Inst()->write("Need scattering factors.");
		throw;
	}
	
	// END OF database section //
};

string Database::guessformat(string filename) {
	// do the best you can to guess the format
	boost::regex e_xml(".*\\.xml$");
	
	if (boost::regex_match(filename,e_xml)) return "xml";
	
	// else: problem
	Err::Inst()->write("Database file format could not be detected (ending with: xml ?)");
	throw;
}

bool Database::check() {
	// implement a sanity check for the database
	return true;
}

void Database::init(std::string filename,std::string format) {
	if (format=="") {
		Info::Inst()->write(string("Detecting format of database file: ") + filename);
		format = guessformat(filename);
		Info::Inst()->write(string("Detected format: ") + format);
	}
	
	if (format=="xml") {
		read_xml(filename);
	}

	Info::Inst()->write("Checking database...");	
	
	if (check()) {
		Info::Inst()->write("Check succeeded. Database seems OK.");			
	}
	else {
		Err::Inst()->write("Check failed. Please check your input file.");
		throw;		
	}
	
}

void Database::write(std::string filename, std::string format) {
	if (format=="") {
		Info::Inst()->write("No database format specified. Will use conf.");
		format = "conf";
	}
	
	if (format=="conf") {
		Err::Inst()->write("conf database format (write) not yet implemented.");
		throw;
	}
	if (format=="xml") {
		Err::Inst()->write("XML database format (write) not yet implemented.");
		throw;
	}
}


void DatabaseNamesPDBParameters::reg(string label, string regexp) {
	m_label2regexp[label]=regexp;
}

void DatabaseNamesPDBParameters::add_quicklookup(string testlabel,string label) {
	if (quicklookup_counter>1000000) clear_quicklookup();
	m_quicklookup[testlabel]=label;
	quicklookup_counter++;	
}

void DatabaseNamesPDBParameters::clear_quicklookup() {
	m_quicklookup.clear();
	quicklookup_counter=0;
}

string DatabaseNamesPDBParameters::get(string testlabel) {
		
   int matches = 0;
   string label = "<Unknown>";

	if (m_quicklookup.find(testlabel)!=m_quicklookup.end()) {
		return m_quicklookup[testlabel];
	}
	
	for(map<string,string>::iterator lri = m_label2regexp.begin();lri!=m_label2regexp.end();lri++ ) {
	   	boost::regex e(lri->second);
	
	   	if (regex_match(testlabel,e)) {
	   		if (matches>0) {
				Err::Inst()->write(string("PDB atom name matches")+string(label)+string(" and ")+string(lri->first));
				Err::Inst()->write(string("expression to match: ")+string(testlabel));
	   			throw;
	   		} else 
			{ 
				label = lri->first; 
				matches++; 
			}
	   	}
	}

   if (matches==0) {
	Err::Inst()->write(string("PDB atom name not recognized: ")+string(testlabel));
   	throw;
   }
   
	m_quicklookup[testlabel]=label;
	return label;	
}



void DatabaseAtomIDsParameters::reg(string label) {
	if (m_IDs.find(label)==m_IDs.end()) {
		m_IDs[label]=nextID;
		m_rIDs[nextID]=label;
		nextID++;
	}
}

size_t DatabaseAtomIDsParameters::get(string label) {
	if (m_IDs.find(label)==m_IDs.end()) {
		reg(label);
	}
	return m_IDs[label];
}

string DatabaseAtomIDsParameters::rget(size_t ID) {
	return m_rIDs[ID];
}


void DatabaseMassesParameters::reg(size_t ID, double value) {
	m_masses[ID]=value;
}

double DatabaseMassesParameters::get(size_t ID) {
	return m_masses[ID];
}




void DatabaseVolumesParameters::reg(size_t ID, vector<double> constants, size_t function_type) {
	m_constants[ID]=constants;
	m_functiontypes[ID] = function_type;
}

void DatabaseVolumesParameters::add_quicklookup(size_t ID,double value) {
	if (quicklookup_counter>1000000) clear_quicklookup();
	m_quicklookup[ID]=value;
	quicklookup_counter++;	
}

void DatabaseVolumesParameters::clear_quicklookup() {
	m_quicklookup.clear();
	quicklookup_counter=0;
}

double DatabaseVolumesParameters::get(size_t ID) {
		
	size_t ft = m_functiontypes[ID];
	vector<double> v = m_constants[ID];
	if (ft==0) { // 0 = constants // volume direct
		return v[0];
	}
	
	if (ft==1) { // hardsphere radius
		return (4.0/3.0)*M_PI*powf(v[0],3);
	}
	
	if (ft==2) { // gausssphere radius
		return sqrt(powf(M_PI,3))*powf(v[0],3);
	}

	// don't need a quicklookup for volume calculations, b/c pretty fast
	// function-type not mapped
    Err::Inst()->write(string("Size-type not implemented: type=")+boost::lexical_cast<string>(ft));
    throw;
}


void DatabaseExlusionParameters::reg(size_t ID, vector<double> constants, size_t function_type) {
	m_constants[ID]=constants;
	m_functiontypes[ID] = function_type;
}

void DatabaseExlusionParameters::add_quicklookup(size_t ID,double value) {
	if (quicklookup_counter>1000000) clear_quicklookup();
	m_quicklookup[ID]=value;
	quicklookup_counter++;	
}

void DatabaseExlusionParameters::clear_quicklookup() {
	m_quicklookup.clear();
	quicklookup_counter=0;
}

double DatabaseExlusionParameters::get(size_t ID,double effvolume,double q) {
		
	size_t ft = m_functiontypes[ID];
	vector<double> v = m_constants[ID];
	if (ft==0) { // 0 = constants 
		return v[0];
	}
	
	if (ft==1) { // 1 = linear
		return effvolume*v[0];
	}
	
	if (ft==2) { // gaussian
        return effvolume*exp(-1.0*powf(effvolume,2.0/3.0)*powf(q,2)/(4*M_PI))*v[0];
	}

	// don't need a quicklookup for volume calculations, b/c pretty fast
	// function-type not mapped
    Err::Inst()->write(string("ExclusionParameter-type not implemented: type=")+boost::lexical_cast<string>(ft));
    throw;
}


void DatabaseSFactorsParameters::reg(size_t ID, vector<double> constants, size_t function_type) {
	m_constants[ID]=constants;
	m_functiontypes[ID] = function_type;
}

void DatabaseSFactorsParameters::add_quicklookup(size_t ID, double q,double value) {
	if (quicklookup_counter>1000000) clear_quicklookup();
	m_quicklookup[q][ID]=value;
	quicklookup_counter++;	
}

void DatabaseSFactorsParameters::clear_quicklookup() {
	m_quicklookup.clear();
	quicklookup_counter=0;
}

double DatabaseSFactorsParameters::get(size_t ID,double q) {
		
	size_t& ft = m_functiontypes[ID];
	vector<double>& v = m_constants[ID];
	if (ft==0) { // 0 = constants
		return v[0];
	}
	
	// a shortcut for expensive calculations, this thing grows. 
	if ((ft==1) || (ft==2)) {
		if ((m_quicklookup.find(q)!=m_quicklookup.end()) && (m_quicklookup[q].find(ID)!=m_quicklookup[q].end())) {
			return m_quicklookup[q][ID];
		}
	}
	
	if (ft==1) {
		// slater:
		//     den=0.0
		//      do j=1,5
		//        if (coef(z,1,j).ne.0.0) then
		//          c = ((2.*coef(z,1,j)/coef(z,3,j))**(coef(z,3,j)+0.5))/
		//     1        sqrt(fak(int(2.*coef(z,3,j))))
		//          den = den+coef(z,2,j)*(      c  *  r**(coef(z,3,j)-1.0)  *  exp(-coef(z,1,j)  *  r/coef(z,3,j)/a)       )**2
		//     1          exp(-coef(z,1,j)*r/coef(z,3,j)/a))**2
		//        end if
		//      end do
		//      den = den/4./pi
		double den = 0.0;
		for(size_t j = 0; j < 5; ++j)
		{
			size_t j2 = 5+j;
			size_t j3 = 10+j;
			if (v[j]!=0.0) {
				double c = 0.0;
				c += powf( 2.0*v[j]/v[j3], v[j3]+0.5 );
				c /= sqrt( boost::math::factorial<double>(2.0*v[j3]) );
				den += v[j2]* powf(  c*powf(q, v[j3]-1.0)*exp(-1.0*v[j]*q/v[j3]/1.0) , 2);
			}
		}
		
		add_quicklookup(ID,q,den / (4.0*M_PI));
		return (den / (4.0*M_PI));
	}
	
	if (ft==2) {
		double den = 0.0;
		den += v[0]*exp(-v[1]*q*q);
		den += v[2]*exp(-v[3]*q*q);
		den += v[4]*exp(-v[5]*q*q);
		den += v[6]*exp(-v[7]*q*q);
		den += v[8];

		add_quicklookup(ID,q,den);
		return den;
	}
	
	// function-type not mapped
    Err::Inst()->write(string("ScatterFactor-type not implemented: type=")+boost::lexical_cast<string>(ft));
    throw;
}