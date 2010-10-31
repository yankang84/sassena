/*
 *  file_writer_service.hpp
 *
 *  Created on: May 26, 2008
 *  Authors:
 *  Benjamin Lindner, ben@benlabs.net
 *
 *  Copyright 2008,2009 Benjamin Lindner
 *
 */

#ifndef IO__FILE_WRITER_SERVICE_HPP_
#define IO__FILE_WRITER_SERVICE_HPP_

// common header
#include "common.hpp"

// standard header
#include <complex>
#include <map>
#include <string>
#include <queue>
#include <vector>

// special library headers
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/date_time.hpp>
#include <hdf5.h>
#include <hdf5_hl.h>

// other headers
#include "math/coor3d.hpp"

enum HDF5WriterTag {HANGUP,WRITE};

class HDF5WriterClient {
    boost::asio::ip::tcp::endpoint m_endpoint;
    boost::posix_time::ptime m_lastflush;
    
    std::queue<std::pair<CartesianCoor3D,std::vector<std::complex<double> >*> > data_queue;
    
public:
    HDF5WriterClient(boost::asio::ip::tcp::endpoint server) : m_endpoint(server) {}
    ~HDF5WriterClient();

    void write(CartesianCoor3D qvector,const std::vector<std::complex<double> >& data);
    void flush();    
};

class HDF5WriterService  {
    
    std::string m_filename;
    boost::posix_time::ptime m_lastflush;

    boost::asio::io_service& m_io_service;
    boost::asio::ip::tcp::acceptor m_acceptor;

    boost::thread* m_listener;
    bool m_listener_status;

//    std::vector<size_t> init(const std::vector<CartesianCoor3D>& qvectors,size_t nf);
//    std::vector<size_t> init_new(const std::vector<CartesianCoor3D>& qvectors,size_t nf);
//    std::vector<size_t> init_reuse(const std::vector<CartesianCoor3D>& qvectors,size_t nf);
    void  init_new(size_t nf);
    void  init(size_t nf);

    bool test_fqt_dim(size_t nf);
    
    void listener();
    
    void flush();

    std::vector< CartesianCoor3D > data_qvectors;    
    std::vector<std::vector<std::complex<double> >*> data_fqt;
    std::vector<std::complex<double> > data_fq;
public:
    
    HDF5WriterService(boost::asio::io_service& io_service, const std::string filename,size_t nf);

    boost::asio::ip::tcp::endpoint get_endpoint() { return m_acceptor.local_endpoint();}
    std::vector<CartesianCoor3D> get_qvectors();
    
    void hangup();
    void run();
};



#endif
