/*
* IPFIX Fastbit Reader/Writer
* Copyright (C) 2011 Philipp Fehre
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 */

#ifdef FB_SUPPORT_ENABLED

#include "IpfixFbWriter.hpp"
#include "common/msg.h"
#include "common/Misc.h"
#include "common/Time.h"

#include <stdexcept>
#include <string.h>
#include <stdlib.h>
#include <sstream>

using namespace std;


/**
 *  Define data input for fastbit
 *  FIXME Missing!
 */ 


/**
 *  Compare two source IDs and check if exporter is the same (i.e., same IP address and observationDomainId
 *  FIXME Missing!
 */
bool IpfixFbWriter::equalExporter() {
  return true;
}

/**
 *  Set the Partition to operate on or create a new one if needed
 *  FIXME Missing!
 */
int IpfixFbWriter::setCurrentPartition() {
  return 1;
}

/**
 *  Create a new Partion
 *  FIXME Missing!
 */
int IpfixFbWriter::createPartition() {
  return 1;
}

/**
 *  Write current data to directory
 *  FIXME Missing!
 */
int IpfixFbWriter::writeToDir() {
  return 1;
}

/**
 *  Handle a data record to save to table
 *  FIXME Missing!
 */
void IpfixFbWriter::processDataRecord() { 

}

/**
 *	Returns the id of the exporter table entry or 0 in the case of an error
 *  FIXME Missing!
 */
int IpfixFbWriter::getExporterID() {
  return 1;
}


/**
 *	Get data of the record is given by the IPFIX_TYPEID
 *  FIXME Missing!
 */
uint64_t IpfixFbWriter::getData() {
  return 1;
}

/*** Public Interface ***/

/**
 *  called on Data Record arrival
 *  FIXME Missing!
 */
void IpfixFbWriter::onDataRecord(IpfixDataRecord* record) {

}

/**
 *  Constructor
 *  FIXME Missing!
 */ 

IpfixFbWriter::IpfixFbWriter(const char* dir, const uint32_t maxColumns, uint16_t observationDomainId) {
  
}

/**
 *  Destructor
 *  FIXME Missing!
 */
IpfixFbWriter::~IpfixFbWriter() {
}

#endif
