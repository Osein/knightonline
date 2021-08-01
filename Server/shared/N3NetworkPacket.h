#pragma once

#include "stdafx.h"
#include <zmq.hpp>

enum class KnightOnlinePacket {
	REQUEST_VERSION = 0x01

};

struct N3NetworkPacket;

struct N3NetworkPacket_RequestVersion: public N3NetworkPacket {
	N3NetworkPacket_RequestVersion() {
		id = KnightOnlinePacket::REQUEST_VERSION;
	}
private:
	uint32_t m_version;

public:
	void SetVersion(uint32_t version) { m_version = version; }
	uint32_t GetVersion() { return m_version; }

	zmq::message_t toNetworkMessage() {
		return zmq::message_t((void*)this, sizeof(N3NetworkPacket_RequestVersion));
	}

	void fromNetworkMessage(zmq::message_t &msg) {
		auto packet = (N3NetworkPacket_RequestVersion*)msg.data();
		m_version = packet->m_version;
	}
};

struct N3NetworkPacket
{
public:
	KnightOnlinePacket id;

	virtual zmq::message_t toNetworkMessage() = 0;
	virtual void fromNetworkMessage(zmq::message_t &msg) = 0;
};