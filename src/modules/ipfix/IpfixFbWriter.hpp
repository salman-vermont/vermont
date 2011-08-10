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

#ifndef IPFIXFBWRITER_H
#define IPFIXFBWRITER_H

#include "IpfixDbCommon.hpp"
#include "IpfixRecordDestination.h"
#include "common/ipfixlolib/ipfix.h"
#include "common/ipfixlolib/ipfixlolib.h"
#include "ibis.h"
#include <netinet/in.h>


#define EXPORTERID 0

/**
 * IpfixFbWriter enables the communication with Fastbit compressed Bitmap indexes
 */
 
class IpfixFbWriter 
  : public IpfixRecordDestination, public Module, public Source<NullEmitable*>
{
  public:
    IpfixFbWriter(const char* dir, 
                  const uint32_t maxColumns,
                  uint16_t observationDomainId,
                  vector<string> rows);
    ~IpfixFbWriter();
    
    void onDataRecord(IpfixDataRecord* record);
    
    IpfixRecord::SourceID srcId; // default SourceID
    
  private:
    static const uint32_t MAX_COLUMNS;
    const char* fastbitDir; // directory holding the fastbit data
    ibis::tablex* tx; // expandable table
    ibis::table* tb; // readable table
        
    // DEBUG OUTPUT
    ibis::util::logger lg; // fastbit logger
    int ierr; // error

    list<string> usedPartions; // contains the partions used so far
    
    // Methods
    int createPartition();
    int setCurrentPartition();
    int writeToDir();
    void processDataRecord();
    int getExporterID();
    uint64_t getData();
    bool equalExporter();
};

#endif

#endif
