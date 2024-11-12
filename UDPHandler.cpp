#include "UDPHandler.h"

#include <string>
#include <cstring>
#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

UDPHandler::UDPHandler(): rtpSocket(-1), rtcpSocket(-1) {}

UDPHandler::~UDPHandler() {
    if (rtpSocket != -1) close(rtpSocket);
    if (rtcpSocket != -1) close(rtcpSocket);
}

bool UDPHandler::createUDPSocket(int port1, int port2) {
    rtpSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (rtpSocket == -1) {
        cerr << "rtp 소켓 생성 실패" << endl;
        return false;
    }

    rtcpSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (rtcpSocket == -1) {
        cerr << "rtcp 소켓 생성 실패" << endl;
        return false;
    }

    memset(&rtpAddr, 0, sizeof(rtpAddr));
    rtpAddr.sin_family = AF_INET;
    rtpAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    rtpAddr.sin_port = htons(port1);

    memset(&rtcpAddr, 0, sizeof(rtcpAddr));
    rtcpAddr.sin_family = AF_INET;
    rtcpAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    rtcpAddr.sin_port = htons(port2);

    return true;
}

void UDPHandler::sendRTPPacket(unsigned char* rtpPacket, size_t packetSize) {
    int sentBytes = sendto(rtpSocket, rtpPacket, packetSize, 0, (struct sockaddr*)&rtpAddr, sizeof(rtpAddr));
    if (sentBytes == -1) {
        std::cerr << "Error: fail to send RTP packet" << std::endl;
        exit(1);
    }
}

void UDPHandler::sendSenderReport(Protos::SenderReport* senderReport, size_t srSize) {
    int sentBytes = sendto(rtcpSocket, senderReport, srSize, 0, (struct sockaddr*)&rtcpAddr, sizeof(rtcpAddr));
    if (sentBytes == -1) {
        std::cerr << "Error: fail to send RTCP packet" << std::endl;
        exit(1);
    }
}

int& UDPHandler::getRTPSocket() { return rtpSocket; }
int& UDPHandler::getRTCPSocket() { return rtcpSocket; }

sockaddr_in& UDPHandler::getRTPAddr() { return rtpAddr; }
sockaddr_in& UDPHandler::getRTCPAddr() { return rtcpAddr; }