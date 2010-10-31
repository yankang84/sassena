/*
 *  parameters.hpp
 *reg
 *  Created on: June 25, 2009
 *  Authors:
 *  Benjamin Lindner, ben@benlabs.net
 *
 *  Copyright 2008,2009 Benjamin Lindner
 *
 */

#ifndef CONTROL__PARAMETERS_HPP_
#define CONTROL__PARAMETERS_HPP_

// common header
#include "common.hpp"

// standard header
#include <iostream>
#include <map>
#include <string>
#include <vector>

// special library headers
#include <boost/serialization/access.hpp>
#include <boost/serialization/map.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/vector.hpp>

// other headers
#include "math/coor3d.hpp"

// This is a wrapper class to interface the settings implementation. The rational is to 
// move all possible configuration errors towards the initialization of the software
// preferably the 'parameters' class checks for all required settings and implementents
// default values
// Also, use hardwired constant names to move possible errors to compile time.
// Basically this class maps the structure of the configuration file, more or less

// these constructs are to be used w/ in the code the following way:
// string fs = Params::Inst()->sample.structure.file

// resolve dependency:
class CartesianCoor3D;


class SampleStructureParameters {
private:
	/////////////////// MPI related
	// make this class serializable to 
	// allow sample to be transmitted via MPI
    friend class boost::serialization::access;	
	template<class Archive> void serialize(Archive & ar, const unsigned int version)
    {
		ar & file;
		ar & format;
    }
	/////////////////// 

public:	
	std::string file;
	std::string format;
};

class SampleGroupParameters {
private:
	/////////////////// MPI related
	// make this class serializable to 
	// allow sample to be transmitted via MPI
    friend class boost::serialization::access;	
	template<class Archive> void serialize(Archive & ar, const unsigned int version)
    {
		ar & name;
		ar & file;
		ar & format;
		ar & select;
		ar & select_value;
    }
	/////////////////// 

public:	
	std::string name;
	std::string file;
	std::string format;
	std::string select;
	double select_value;
	
	SampleGroupParameters() {}
	SampleGroupParameters(const std::string& v1,const std::string& v2,const std::string& v3,const std::string& v4,const double& v5) {
		name = v1; file = v2; format = v3; select = v4; select_value = v5; 
	}
};

class SampleFramesetParameters {
private:
	/////////////////// MPI related
	// make this class serializable to 
	// allow sample to be transmitted via MPI
    friend class boost::serialization::access;	
	template<class Archive> void serialize(Archive & ar, const unsigned int version)
    {
		ar & first;
		ar & last;
		ar & last_set;
		ar & stride;
		ar & filename;
		ar & type;
        ar & clones;
        ar & index;
    }
	/////////////////// 

public:	
	size_t first;
	size_t last;
    size_t clones;
	bool last_set;
	size_t stride;
	std::string filename;
	std::string type;
    std::string index;
};


class SampleFramesetsParameters : public std::vector<SampleFramesetParameters > {
private:
	/////////////////// MPI related
	// make this class serializable to 
	// allow sample to be transmitted via MPI
    friend class boost::serialization::access;	
	template<class Archive> void serialize(Archive & ar, const unsigned int version)
    {
		ar & boost::serialization::base_object<std::vector<SampleFramesetParameters>, SampleFramesetsParameters>(*this);
    }
	/////////////////// 

public:	
};

class SampleMotionParameters {
private:
	/////////////////// MPI related
	// make this class serializable to 
	// allow sample to be transmitted via MPI
    friend class boost::serialization::access;	
	template<class Archive> void serialize(Archive & ar, const unsigned int version)
    {
		ar & type;
		ar & displace;
		ar & direction;
		ar & frequency;
        ar & radius;
		ar & selection;
		ar & seed;
        ar & sampling;
    }
	/////////////////// 

public:	
	std::string type;
	double displace;
	double frequency;
    double radius;
	std::string selection;
	long seed;
	long sampling;	
	CartesianCoor3D direction;
};


class SampleAlignmentParameters {
private:
	/////////////////// MPI related
	// make this class serializable to 
	// allow sample to be transmitted via MPI
    friend class boost::serialization::access;	
	template<class Archive> void serialize(Archive & ar, const unsigned int version)
    {
		ar & type;
		ar & selection;
        ar & order;
    }
	/////////////////// 

public:	
	std::string type;
	std::string selection;
    std::string order;
};

class SampleParameters {
private:
	/////////////////// MPI related
	// make this class serializable to 
	// allow sample to be transmitted via MPI
    friend class boost::serialization::access;	
	template<class Archive> void serialize(Archive & ar, const unsigned int version)
    {
		ar & structure;
		ar & groups;
		ar & framesets;
		ar & motions;
        ar & alignments;
    }
	/////////////////// 

public:	
	SampleStructureParameters structure;
	std::map<std::string,SampleGroupParameters> groups;
	SampleFramesetsParameters framesets;
	std::vector<SampleMotionParameters> motions;
    std::vector<SampleAlignmentParameters> alignments;
};

class ScatteringBackgroundKappaParameters {
private:
	/////////////////// MPI related
	// make this class serializable to 
	// allow sample to be transmitted via MPI
    friend class boost::serialization::access;	
	template<class Archive> void serialize(Archive & ar, const unsigned int version)
    {
        ar & selection;
        ar & value;
    }
	/////////////////// 

public:	
	std::string selection;
	double value;
};

class ScatteringBackgroundParameters {
private:
	/////////////////// MPI related
	// make this class serializable to 
	// allow sample to be transmitted via MPI
    friend class boost::serialization::access;	
	template<class Archive> void serialize(Archive & ar, const unsigned int version)
    {
		ar & type;
		ar & factor;
		ar & kappas;
    }
	/////////////////// 

public:	
    std::string type;
    double factor;
    
    std::vector<ScatteringBackgroundKappaParameters> kappas;
};


class ScatteringAverageOrientationVectorsParameters : public std::vector<CartesianCoor3D> {
private:
	/////////////////// MPI related
	// make this class serializable to 
	// allow sample to be transmitted via MPI
    friend class boost::serialization::access;	
	template<class Archive> void serialize(Archive & ar, const unsigned int version)
    {
		ar & boost::serialization::base_object<std::vector<CartesianCoor3D> >(*this);
		ar & type;
		ar & algorithm;
		ar & resolution;
		ar & file;
		ar & axis;
		ar & seed;
    }
	/////////////////// 

public:	
	std::string type;
	std::string algorithm;
	std::string file;
	CartesianCoor3D axis;
	size_t resolution;
	long seed;
	
	void create();
};

class ScatteringAverageOrientationMultipoleParameters {
private:
	/////////////////// MPI related
	// make this class serializable to 
	// allow sample to be transmitted via MPI
    friend class boost::serialization::access;	
	template<class Archive> void serialize(Archive & ar, const unsigned int version)
    {
		ar & type;
		ar & resolution;
		ar & axis;
    }
	/////////////////// 

public:	
	std::string type;
	long resolution;
	CartesianCoor3D axis;
};

class ScatteringAverageOrientationExactParameters {
private:
	/////////////////// MPI related
	// make this class serializable to 
	// allow sample to be transmitted via MPI
    friend class boost::serialization::access;	
	template<class Archive> void serialize(Archive & ar, const unsigned int version)
    {
		ar & type;
    }
	/////////////////// 

public:	
	std::string type; 
};

class ScatteringAverageOrientationParameters {
private:
	/////////////////// MPI related
	// make this class serializable to 
	// allow sample to be transmitted via MPI
    friend class boost::serialization::access;	
	template<class Archive> void serialize(Archive & ar, const unsigned int version)
    {
		ar & type;
		ar & vectors;
		ar & multipole;
		ar & exact;
    }
	/////////////////// 
	
public:		
	std::string type; 
	ScatteringAverageOrientationVectorsParameters vectors;
	ScatteringAverageOrientationMultipoleParameters multipole;
	ScatteringAverageOrientationExactParameters exact;

};

class ScatteringAverageParameters {
private:
	/////////////////// MPI related
	// make this class serializable to 
	// allow sample to be transmitted via MPI
    friend class boost::serialization::access;	
	template<class Archive> void serialize(Archive & ar, const unsigned int version)
    {
		ar & orientation;
    }
	/////////////////// 

public:	
	ScatteringAverageOrientationParameters orientation;
};

class ScatteringDspParameters {
private:
	/////////////////// MPI related
	// make this class serializable to 
	// allow sample to be transmitted via MPI
    friend class boost::serialization::access;	
	template<class Archive> void serialize(Archive & ar, const unsigned int version)
    {
		ar & type;
		ar & method;
    }
	/////////////////// 

public:	
	std::string type;
	std::string method;
};

class ScatteringVectorsScanParameters {
private:
	/////////////////// MPI related
	// make this class serializable to 
	// allow sample to be transmitted via MPI
    friend class boost::serialization::access;	
	template<class Archive> void serialize(Archive & ar, const unsigned int version)
    {
		ar & from;
		ar & to;
		ar & points;
		ar & exponent;
		ar & basevector;	
    }
	/////////////////// 

public:	
	ScatteringVectorsScanParameters() : points(100), exponent(1.0), basevector(CartesianCoor3D(1,0,0)) {}
	
	double from;
	double to;
	size_t points;
	double exponent;
	CartesianCoor3D basevector;
};

class ScatteringVectorsParameters : public std::vector<CartesianCoor3D> {
private:
	/////////////////// MPI related
	// make this class serializable to 
	// allow sample to be transmitted via MPI
    friend class boost::serialization::access;	
	template<class Archive> void serialize(Archive & ar, const unsigned int version)
    {
		ar & boost::serialization::base_object<std::vector<CartesianCoor3D> >(*this);
		ar & scans;
    }
	/////////////////// 

public:
	std::vector<ScatteringVectorsScanParameters> scans;
	
	void create_from_scans();
};

class ScatteringSignalParameters {
private:
	/////////////////// MPI related
	// make this class serializable to 
	// allow sample to be transmitted via MPI
    friend class boost::serialization::access;	
	template<class Archive> void serialize(Archive & ar, const unsigned int version)
    {
		ar & file;
        ar & fqt;
        ar & fq;
        ar & fq0;
        ar & fq2;
    }
	/////////////////// 

public:
	std::string file;
    bool fqt;
    bool fq;
    bool fq0;
    bool fq2;
};

class ScatteringParameters {
private:
	/////////////////// MPI related
	// make this class serializable to 
	// allow sample to be transmitted via MPI
    friend class boost::serialization::access;	
	template<class Archive> void serialize(Archive & ar, const unsigned int version)
    {
        ar & type;
		ar & dsp;
		ar & target;
		ar & qvectors;
		ar & average;
		ar & background;
        ar & signal;
    }
	/////////////////// 

public:	
	std::string type;
	ScatteringDspParameters dsp;
	std::string target;

	ScatteringVectorsParameters qvectors;
	
	ScatteringAverageParameters average;
	ScatteringBackgroundParameters background;	

	ScatteringSignalParameters signal;	
};



class LimitsComputationParameters {
private:
	/////////////////// MPI related
	// make this class serializable to 
	// allow sample to be transmitted via MPI
    friend class boost::serialization::access;	
	template<class Archive> void serialize(Archive & ar, const unsigned int version)
    {
		ar & threads;
		ar & worker1_threads;
		ar & worker3_threads;
		
    }
	/////////////////// 

public:
    bool threads;
    size_t worker1_threads;
    size_t worker3_threads;
};

class LimitsMemoryParameters {
private:
	/////////////////// MPI related
	// make this class serializable to 
	// allow sample to be transmitted via MPI
    friend class boost::serialization::access;	
	template<class Archive> void serialize(Archive & ar, const unsigned int version)
    {
		ar & scattering_matrix;
        ar & coordinate_sets;
        ar & data;
        ar & iowrite_client;
        ar & iowrite_server;
        ar & at1_buffer;     
        ar & data_stager;   
    }
	/////////////////// 

public:
    size_t scattering_matrix;
    size_t coordinate_sets;
    size_t data;
    size_t iowrite_client;
    size_t iowrite_server;    
    size_t at1_buffer;
    size_t data_stager;
};

class LimitsTimesParameters {
private:
	/////////////////// MPI related
	// make this class serializable to 
	// allow sample to be transmitted via MPI
    friend class boost::serialization::access;	
	template<class Archive> void serialize(Archive & ar, const unsigned int version)
    {
        ar & iowrite_client;
        ar & iowrite_server;        
        ar & at1_buffer;
    }
	/////////////////// 

public:
    size_t iowrite_client;
    size_t iowrite_server;    
    size_t at1_buffer;
};

class LimitsDecompositionPartitionsParameters {
private:
	/////////////////// MPI related
	// make this class serializable to 
	// allow sample to be transmitted via MPI
    friend class boost::serialization::access;	
	template<class Archive> void serialize(Archive & ar, const unsigned int version)
    {
        ar & automatic;        
        ar & size;
    }
	/////////////////// 

public:
    bool automatic;
    size_t size;
    
};

class LimitsDecompositionParameters {
private:
	/////////////////// MPI related
	// make this class serializable to 
	// allow sample to be transmitted via MPI
    friend class boost::serialization::access;	
	template<class Archive> void serialize(Archive & ar, const unsigned int version)
    {
		ar & utilization;
        ar & partitions;        
    }
	/////////////////// 

public:
    double utilization;
    LimitsDecompositionPartitionsParameters partitions;
};

class LimitsSignalParameters {
private:
	/////////////////// MPI related
	// make this class serializable to 
	// allow sample to be transmitted via MPI
    friend class boost::serialization::access;	
	template<class Archive> void serialize(Archive & ar, const unsigned int version)
    {
        ar & alloc_early;      
    }
	/////////////////// 

public:
    bool alloc_early;
};

class LimitsDataParameters {
private:
	/////////////////// MPI related
	// make this class serializable to 
	// allow sample to be transmitted via MPI
    friend class boost::serialization::access;	
	template<class Archive> void serialize(Archive & ar, const unsigned int version)
    {
		ar & servers;
    }
	/////////////////// 

public:
    size_t servers;
};


class LimitsParameters {
private:
	/////////////////// MPI related
	// make this class serializable to 
	// allow sample to be transmitted via MPI
    friend class boost::serialization::access;	
	template<class Archive> void serialize(Archive & ar, const unsigned int version)
    {
        ar & data;
        ar & signal;
		ar & memory;
        ar & times;
        ar & decomposition;
        ar & computation;
    }
	/////////////////// 

public:
    LimitsDataParameters data;    
    LimitsSignalParameters signal;    
    LimitsMemoryParameters memory;
    LimitsTimesParameters times;
    LimitsComputationParameters computation;    
    LimitsDecompositionParameters decomposition;
};

class DebugMonitorParameters {
   private:
   	/////////////////// MPI related
   	// make this class serializable to 
   	// allow sample to be transmitted via MPI
       friend class boost::serialization::access;	
   	template<class Archive> void serialize(Archive & ar, const unsigned int version)
       {
   		ar & update;
       }
   	/////////////////// 

   public:
   	bool update; 
};

class DebugPrintParameters {
private:
	/////////////////// MPI related
	// make this class serializable to 
	// allow sample to be transmitted via MPI
    friend class boost::serialization::access;	
	template<class Archive> void serialize(Archive & ar, const unsigned int version)
    {
        ar & orientations;
    }
	/////////////////// 

public:
	bool orientations;
};


class DebugIowriteParameters {
private:
	/////////////////// MPI related
	// make this class serializable to 
	// allow sample to be transmitted via MPI
    friend class boost::serialization::access;	
	template<class Archive> void serialize(Archive & ar, const unsigned int version)
    {
        ar & write;
        ar & buffer;
    }
	/////////////////// 

public:
	bool write;
    bool buffer;
};

class DebugParameters {
private:
	/////////////////// MPI related
	// make this class serializable to 
	// allow sample to be transmitted via MPI
    friend class boost::serialization::access;	
	template<class Archive> void serialize(Archive & ar, const unsigned int version)
    {
		ar & timer;
		ar & barriers;
        ar & monitor;
        ar & iowrite;
        ar & print;
    }
	/////////////////// 

public:
	bool timer;
	bool barriers;
    DebugIowriteParameters iowrite;
    DebugPrintParameters print;
    DebugMonitorParameters monitor;
};




// implement Parameters as a singleton class, this makes it globally available
// it requires the call of the init() function, which also implements all checks
class Params  {
private:
	/////////////////// MPI related
	// make this class serializable to 
	// allow sample to be transmitted via MPI
    friend class boost::serialization::access;	
	template<class Archive> void serialize(Archive & ar, const unsigned int version)
    {
		ar & carboncopy;
		ar & config_rootpath;
		ar & sample;
		ar & scattering;
		ar & limits;
		ar & debug;
    }
	/////////////////// 

	Params() {}
	Params(const Params&);
	Params& operator=(const Params&);

	std::vector<std::string> carboncopy;
	
	std::string config_rootpath;
	
	std::string get_filepath(std::string filename);	

	void read_xml(std::string filename);
	
public: 
	// interface for parameters
	SampleParameters sample;
	ScatteringParameters scattering;
	LimitsParameters limits;
	DebugParameters debug;
	
	// interface for initiatilzation and interfacing
	static Params* Inst() { static Params instance; return &instance;}
	
	void init(std::string filename);
	~Params() {}; // it is said some compilers have problems w/ private destructors.
	
	void write_xml(std::string filename);	
};

#endif 

// end of file
