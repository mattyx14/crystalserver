//////////////////////////////////////////////////////////////////////
// OpenTibia - an opensource roleplaying game
//////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
//////////////////////////////////////////////////////////////////////
#include "otpch.h"

#include "definitions.h"

#if defined WIN32
#include <winerror.h>
#endif

#include "protocol.h"
#include "connection.h"
#include "outputmessage.h"
#include "tools.h"

void Protocol::onSendMessage(OutputMessage* msg)
{
	#ifdef __DEBUG_NET_DETAIL__
	std::cout << "Protocol::onSendMessage" << std::endl;
	#endif

	if(!m_rawMessages)
		msg->writeMessageLength();

	if(msg == m_outputBuffer)
		m_outputBuffer = NULL;
}

void Protocol::onRecvMessage(NetworkMessage& msg)
{
	#ifdef __DEBUG_NET_DETAIL__
	std::cout << "Protocol::onRecvMessage" << std::endl;
	#endif

	parsePacket(msg);
}

OutputMessage* Protocol::getOutputBuffer()
{
	if(m_outputBuffer)
		return m_outputBuffer;
	else if(m_connection)
	{
		m_outputBuffer = OutputMessagePool::getInstance()->getOutputMessage(this);
		return m_outputBuffer;
	}
	else
		return NULL;
}

void Protocol::deleteProtocolTask()
{
	//dispather thread
	if(m_outputBuffer)
		OutputMessagePool::getInstance()->releaseMessage(m_outputBuffer);

	delete this;
}

uint32_t Protocol::getIP() const
{
	if(getConnection())
		return getConnection()->getIP();

	return 0;
}
