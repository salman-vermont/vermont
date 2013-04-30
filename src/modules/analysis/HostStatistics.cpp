/*
 * VERMONT
 * Copyright (C) 2009 Matthias Segschneider <matthias.segschneider@gmx.de>
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
 */


#include "HostStatistics.h"
#include "modules/ipfix/Connection.h"
#include "common/Misc.h"

#include <netdb.h>
#include <fstream>

//#include <SrcDstIP.h>

InstanceManager<Host> HostStatistics::hostManager("Host");

HostStatistics::HostStatistics(std::string ipSubnet, std::string addrFilter, std::string logPath, uint16_t logInt, uint16_t classificationThrldCli, double classificationThrldSrc, uint16_t classificationThrldNoOfFlows)
	: ipSubnet(ipSubnet), addrFilter(addrFilter), logPath(logPath), logInt(logInt), classificationThrldCli(classificationThrldCli), classificationThrldSrc (classificationThrldSrc), classificationThrldNoOfFlows(classificationThrldNoOfFlows)
{
	// check if srcIP or dstIP in the subnet (1.1.1.1/16)
	// split string at the '/'
	size_t found = ipSubnet.find_first_of("/");
	std::string ip_str = ipSubnet.substr(0, found);
	netSize = atoi(ipSubnet.substr(found + 1).c_str());
	netAddr = *(uint32_t *)gethostbyname(ip_str.c_str())->h_addr;
	logTimer = time(NULL);
}

HostStatistics::~HostStatistics()
{
	dumpStatistics();
}

void HostStatistics::onDataRecord(IpfixDataRecord* record)
{
	
	// only treat non-Options Data Records (although we cannot be sure that there is a Flow inside)
	if((record->templateInfo->setId != TemplateInfo::NetflowTemplate) 
		&& (record->templateInfo->setId != TemplateInfo::IpfixTemplate) 
		&& (record->templateInfo->setId != TemplateInfo::IpfixDataTemplate)) {
		record->removeReference();
		return;
	}
	
	Connection conn(record);
	HostMap::iterator it;
	DistinctHostMap::iterator it1;
	uint64_t srcdstip;
	uint64_t dstsrcip;
	

//	FILE* logFile = fopen("host_stats.log", "a");

	if ((addrFilter == "src" || addrFilter == "both") && ((conn.srcIP&netAddr) == netAddr)) {
		it = hostMap.find(conn.srcIP);
		if (it == hostMap.end())	{
			Host* h = hostManager.getNewInstance();
			h->setIP(conn.srcIP);
			h->noOfDistinctSrcPorts = 0;
			h->noOfDistinctDstPorts = 0;
			it = hostMap.insert(pair<uint32_t, Host*>(conn.srcIP, h)).first;
		}
		it->second->addConnection(&conn);


			

	} 
	if ((addrFilter == "dst" || addrFilter == "both") && ((conn.dstIP&netAddr) == netAddr)) {
		it = hostMap.find(conn.dstIP);
		if (it == hostMap.end()) {
			Host* h = hostManager.getNewInstance();
			h->setIP(conn.dstIP);
			h->noOfDistinctSrcPorts = 0;
			h->noOfDistinctDstPorts = 0;
			it = hostMap.insert(pair<uint32_t, Host*>(conn.dstIP, h)).first;
		} 
		it->second->addConnection(&conn);
	}

	/* Concatinate IP pair of source IP <--> destination IP and the otherway around and then create iterator and update the host 		statistics record  */
	srcdstip = (uint64_t)conn.srcIP << 32 | conn.dstIP;
	dstsrcip = (uint64_t)conn.dstIP << 32 | conn.srcIP;
	it1 = distincthostMap.find(srcdstip);
	if(it1 == distincthostMap.end()) {
		Host* h = hostManager.getNewInstance();
		h->setIP(conn.srcIP);
		h->noOfDistinctSrcPorts = 0;
		h->noOfDistinctDstPorts = 0;
		it1 = distincthostMap.insert(pair<uint64_t, Host*>(srcdstip, h)).first;	
		it1->second->addConnection(&conn);

		Host* h1 = hostManager.getNewInstance();
		h1->setIP(conn.dstIP);
		h1->noOfDistinctSrcPorts = 0;
		h1->noOfDistinctDstPorts = 0;
		it1 = distincthostMap.insert(pair<uint64_t, Host*>(dstsrcip, h1)).first;
		it1->second->addConnection(&conn);
	}
	else {
		it1->second->setIP(conn.srcIP);
		it1->second->addConnection(&conn);
		it1 = distincthostMap.find(dstsrcip);
		it1->second->setIP(conn.dstIP);
		it1->second->addConnection(&conn);
	}
	  
}

void HostStatistics::onReconfiguration1()
{
	dumpStatistics();
}

void HostStatistics::dumpStatistics()
{
	DistinctHostMap::iterator it1;
	HostMap::iterator it;
	
	std::fstream outfile("/home/salmankhan/vermont/test.txt", fstream::out);
	std::fstream outfileSer("/home/salmankhan/vermont/server.txt", fstream::out);
	std::fstream outfileCli("/home/salmankhan/vermont/client.txt", fstream::out);
	std::fstream outfileP2P("/home/salmankhan/vermont/P2P.txt", fstream::out);
	std::fstream outfileUndecided("/home/salmankhan/vermont/Undecided.txt", fstream::out);
	outfile << "# ip answeredFlows unansweredFlows sentBytes sentPackets recBytes recPackets recHighPorts sentHighports recLowPorts sentLowPorts distinctSrcPorts DistinctDstPorts" << std::endl;
	
	// for each element in ipList, write an entry like: IP:Bytesum
	for (it = hostMap.begin(); it != hostMap.end(); it++) {
		Host* h = it->second;
		int no_of_distinct_hosts = 0;
		outfile <<std::endl<< IPToString(it->first).c_str() << " "
			<< h->answeredFlows << " "
			<< h->unansweredFlows << " "
			<< h->sentBytes << " " 
			<< h->sentPackets << " "
			<< h->recBytes << " "
			<< h->recPackets << " "
			<< h->recHighPorts << " "
			<< h->sentHighPorts << " "
			<< h->recLowPorts << " " 
			<< h->sentLowPorts << " "
			<< h->noOfDistinctSrcPorts<<" "
			<< h->noOfDistinctDstPorts;


		for (it1 = distincthostMap.begin(); it1 != distincthostMap.end(); it1++) {
			Host* h1 = it1->second;
			uint32_t sourceIP = (it1->first >> 32) & 0xFFFFFFFF;
			uint32_t destinationIP = (it1->first) & 0xFFFFFFFF;
			//std::cout<< it1->first << " "<< sourceIP <<endl;
			if (sourceIP == it->first) {
				no_of_distinct_hosts++;
			

		
				outfile << " <--> " << IPToString(destinationIP).c_str() << " "
					<< h1->answeredFlows << " "
					<< h1->unansweredFlows << " "
					<< h1->sentBytes << " " 
					<< h1->sentPackets << " "
					<< h1->recBytes << " "
					<< h1->recPackets << " "
					<< h1->recHighPorts << " "
					<< h1->sentHighPorts << " "
					<< h1->recLowPorts << " " 
					<< h1->sentLowPorts;
					/* indentify wheather the Hosts/IPs this source is communicating with are server,client or P2P
					This is the classification of distinct hosts communicating with this IP. Number of flow should be 						greater than a threshold "classificationThrldNoOfFlows" to make the decision.*/
					if ((h1->answeredFlows + h1->unansweredFlows) >= classificationThrldNoOfFlows) {
						if((double)h1->noOfDistinctDstPorts/h1->noOfDistinctSrcPorts > classificationThrldSrc) {
							outfile << "  client";
						}
						else if ((double)h1->noOfDistinctSrcPorts/h1->noOfDistinctDstPorts > classificationThrldCli){
							outfile << "  server";
						}
						else {
							outfile << "  P2P";
						}
					}
					else {
						outfile << " Undecided";
					}
					outfile<< std::endl << "\t \t \t \t \t \t";
			}
		}
		outfile<< std::endl<< "number of distinct hosts " << no_of_distinct_hosts << std::endl << std::endl;
		

 		/*indentify wheather this host is a server, client or P2P by looking in its overall traffic. Number of flow should be 			greater than a threshold "classificationThrldNoOfFlows" to make the decision. */

		if ((h->answeredFlows + h->unansweredFlows) >= classificationThrldNoOfFlows){
			// Check, is it a server?
			if((double)h->noOfDistinctDstPorts/h->noOfDistinctSrcPorts > classificationThrldSrc) {
				outfileSer << "IP " << IPToString(it->first).c_str() << "\t\t no of services " << h->noOfDistinctSrcPorts<< " (";
				for (int i = 0; i < h->noOfDistinctSrcPorts; i++){
 					outfileSer  << ntohs(h->srcPortRecord[i]) << " ";
				}
				outfileSer<< ")\tdst port "<< h->noOfDistinctDstPorts<< "\t distinct hosts "<< no_of_distinct_hosts << std::endl;
			}
			// or is it a client
			else if ((double)h->noOfDistinctSrcPorts/h->noOfDistinctDstPorts > classificationThrldCli){
				outfileCli << "IP " << IPToString(it->first).c_str() << "\t\t src port " << h->noOfDistinctSrcPorts 
				<< "\tdst port "<< h->noOfDistinctDstPorts <<"\t distinct hosts "<< no_of_distinct_hosts << std::endl;
		
			}
			// otherwise it a P2P
			else {
				outfileP2P << "IP " << IPToString(it->first).c_str() << "\t\t src port " << h->noOfDistinctSrcPorts 
				<< "\tdst port "<< h->noOfDistinctDstPorts <<"\t distinct hosts "<< no_of_distinct_hosts << std::endl;
			}
		}
		// if number of flows are not sufficient than "Undecided"
		else {
			outfileUndecided<< "IP " << IPToString(it->first).c_str() << "\t\t src port " << h->noOfDistinctSrcPorts 
			<< "\tdst port "<< h->noOfDistinctDstPorts <<"\t distinct hosts "<< no_of_distinct_hosts << std::endl;
		}

	}
	
}
