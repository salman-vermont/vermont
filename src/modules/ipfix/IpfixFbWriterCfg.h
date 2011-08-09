/*
 * Vermont Configuration Subsystem
 * Copyright (C) 2009 Vermont Project
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

#ifndef IPFIXFBWRITERCFG_H_
#define IPFIXFBWRITERCFG_H_

#ifdef FB_SUPPORT_ENABLED

#define __STDC_LIMIT_MACROS

#include <core/XMLElement.h>
#include <core/Cfg.h>

#include "modules/ipfix/IpfixFbWriter.hpp"

#include <string>
#include <stdint.h>
#include <limits.h> 


using namespace std;


class IpfixFbWriterCfg
	: public CfgHelper<IpfixFbWriter, IpfixFbWriterCfg>
{
public:
	friend class ConfigManager;
	
	virtual IpfixFbWriterCfg* create(XMLElement* e);
	virtual ~IpfixFbWriterCfg();
	
	virtual IpfixFbWriter* createInstance();
	virtual bool deriveFrom(IpfixFbWriterCfg* old);
	
protected:
	
	const char* directory; /**< directory to hold Fastbit Data */
	uint32_t maxCol; /**< maximum number of columns per Fasbit Partition */
	uint16_t observationDomainId;	/**< default observation domain id (overrides the one received in the records */

	IpfixFbWriterCfg(XMLElement*);
};


#endif /*FB_SUPPORT_ENABLED*/

#endif /*IPFIXFBWRITERCFG_H_*/
