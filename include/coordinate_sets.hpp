/*
 *  coordinate_sets.hpp
 *
 *  Created on: Dec 30, 2008
 *  Authors:
 *  Benjamin Lindner, ben@benlabs.net
 *
 *  Copyright 2008,2009 Benjamin Lindner
 *
 */

#ifndef COORDINATESETS_HPP_
#define COORDINATESETS_HPP_

// common header
#include "common.hpp"

// standard header
#include <string>
#include <vector>

// special library headers

// other headers
#include "atomselection.hpp"
#include "sample.hpp"
#include "frames.hpp"
#include "coordinate_set.hpp"
#include "motion_walker.hpp"

//forward declaration...

////////////////////////////////////////////////////////////////////////////////
// 
////////////////////////////////////////////////////////////////////////////////

// This class is used by Scatterdevices to manage coordinate sets
class CoordinateSets {
protected:
	std::map<size_t,CoordinateSet*> setcache;

	Sample* p_sample;
	Atomselection* p_selection;
	
	size_t currentframe_i;
	
	std::vector< std::pair<std::string,MotionWalker*> > m_motion_walkers;
	
	void load_into_cache(size_t framenumber);
	
public:
	CoordinateSets();
	~CoordinateSets() ;

	CoordinateSet& current();
	CoordinateSet& load(size_t frame);	
	void clear_cache();
	
	// use these to initialize the coordinate set:
	void set_selection(Atomselection& selection);
	void set_sample(Sample& sample);
	
    void write_xyz(std::string filename); // dumps xyz coordinates to file
	
	Atomselection& get_selection();
};

// for multipole spherical:
class CoordinateSetsMS : public CoordinateSets {
	Atomselection* p_origin_selection;
public:
	
	void set_origin(Atomselection& origin);
		
	CoordinateSet& load(size_t frame);	// this will be overloaded, we need to cache coordinate transformation at this stage
		
};

// for multipole cylindrical:
class CoordinateSetsMC : public CoordinateSets {
	Atomselection* p_origin_selection;
public:
	
	void set_origin(Atomselection& origin);
		
	CoordinateSet& load(size_t frame);	// this will be overloaded, we need to cache coordinate transformation at this stage
		
};


#endif
// end of file