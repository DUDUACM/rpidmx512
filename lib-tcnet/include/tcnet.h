/**
 * @file tcnet.h
 *
 */
/* Copyright (C) 2019-2024 by Arjan van Vught mailto:info@gd32-dmx.org
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:

 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.

 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#ifndef TCNET_H_
#define TCNET_H_

#include <cstdint>

#include "tcnetpackets.h"
#include "tcnettimecode.h"

namespace tcnet {
enum class Layer {
	LAYER_1,
	LAYER_2,
	LAYER_3,
	LAYER_4,
	LAYER_A,
	LAYER_B,
	LAYER_M,
	LAYER_C,
	LAYER_UNDEFINED
};
}  // namespace tcnet


class TCNet {
public:
	TCNet();
	~TCNet();

	void Start();
	void Stop();

	void Run();

	void Print();

	TTCNetNodeType GetNodeType() const {
		return static_cast<TTCNetNodeType>(m_PacketOptIn.ManagementHeader.NodeType);
	}

	void SetNodeName(const char *pNodeName);
	const char *GetNodeName() {
		return reinterpret_cast<char*>(m_PacketOptIn.ManagementHeader.NodeName);
	}

	void SetLayer(const tcnet::Layer layer);
	tcnet::Layer GetLayer() const {
		return m_Layer;
	}

	void SetUseTimeCode(bool bUseTimeCode) {
		m_bUseTimeCode = bUseTimeCode;
	}
	bool GetUseTimeCode() const {
		return m_bUseTimeCode;
	}

	void SetTimeCodeType(TTCNetTimeCodeType tType);
	TTCNetTimeCodeType GetTimeCodeType() const {
		return m_tTimeCodeType;
	}

	void SetTimeCodeHandler(TCNetTimeCode *pTCNetTimeCode) {
		m_pTCNetTimeCode = pTCNetTimeCode;
	}

public:
	static char GetLayerName(const tcnet::Layer layer);
	static tcnet::Layer GetLayer(const char nChar);

	static TCNet *Get() {
		return s_pThis;
	}

private:
	void HandlePort60000Incoming();
	void HandlePort60001Incoming();
	void HandlePort60002Incoming();
	void HandlePortUnicastIncoming();
	void HandleOptInOutgoing();

	void DumpManagementHeader();
	void DumpOptIn();

private:
	struct TCNetBroadcast {
		static constexpr uint16_t PORT_0 = 60000;
		static constexpr uint16_t PORT_1 = 60001;
		static constexpr uint16_t PORT_2 = 60002;
	};
	struct TCNETUnicast {
		static constexpr uint16_t PORT = 65023;
	};

	int32_t m_aHandles[4];
	TTCNetPacketOptIn m_PacketOptIn;
	uint8_t *m_pReceiveBuffer { nullptr };
	uint32_t m_nIpAddressFrom;
	uint32_t m_nCurrentMillis { 0 };
	uint32_t m_nPreviousMillis { 0 };
	uint32_t m_nLxTimeOffset { 0 };
	uint32_t m_nLxTimeCodeOffset { 0 };
	TCNetTimeCode *m_pTCNetTimeCode { nullptr };
	tcnet::Layer m_Layer = tcnet::Layer::LAYER_M;
	bool m_bUseTimeCode = false;
	float m_fTypeDivider { 1000.0F / 30 };
	TTCNetTimeCodeType m_tTimeCodeType;
	uint8_t m_nSeqTimeMessage { 0 };

	static TCNet *s_pThis;
};

#endif /* TCNET_H_ */
