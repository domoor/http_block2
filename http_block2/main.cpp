#include <winsock2.h>
#include <stdio.h>				// printf()
#include <cstdlib>				// strtoull
#include <vector>				// vector
#include <regex>				// URL_pattern
#include <fstream>				// file in/out
#include <string>				// string
//#include <unordered_map>			// hash_map // not Used

#include "windivert.h"
#include "net_hdr.h"				// tcp/ip header
#include "http_block.h"
//#include "source.h"				// Use if required

#define MAXBUF			0xFFFF		// packet maximum size
#define FILENAME		"hash.txt"	// Using file
using namespace std;

struct return_packet pkt;
regex pattern("Host: ([^\r]+)");  		// windows pattern
//regex pattern("Host: ([^\n]+)");		// linux pattern
vector<uint128_t> value[TABLESIZE];		// URL value
//std::unordered_map<uint16_t, vector<uint128_t>> black_list; // not Used

bool load_txt(std::string in_file) {
	std::ifstream in(in_file);
	std::string str_md5;

	bool flag = 1;
	puts("\nLoading text file...");
	if (in.is_open()) {
		while (!in.eof()) {
			std::getline(in, str_md5);
			uint128_t url_md5;
			uint16_t key = make_md5(str_md5, &url_md5, TABLESIZE);

			value[key].push_back(url_md5);
		}
		flag = 0;
	}
	in.close();

	if (flag == 0)	puts("Loading complete.\n");
	else		fprintf(stderr, "error: File not found\n");
	return flag;
}

void filter(HANDLE handle) {
	static string html = "HTTP/1.0 302 Redirect\r\n"
			     "Location: http://warning.or.kr/i1.html\r\n\r\n";
	uint32_t pkt_len = sizeof(struct return_packet);
	memcpy(pkt.payload, html.c_str(), html.size());

	while (TRUE) {
		uint8_t packet[MAXBUF];
		uint32_t packet_len;
		WINDIVERT_ADDRESS addr;
		if (!WinDivertRecv(handle, packet, sizeof(packet), &addr, &packet_len)) {
			fprintf(stderr, "warning: failed to read packet\n");
			continue;
		}

		struct ipv4_hdr *ip_p = (struct ipv4_hdr*)packet;
		if (ip_p->protocol != IPPROTO_TCP) continue;

		uint32_t ip_len = (ip_p->HL) << 2;
		struct tcp_hdr *tcp_p = (struct tcp_hdr*)(packet + ip_len);
		if (ntohs(tcp_p->d_port) != IPPORT_HTTP) continue;

		uint32_t tcp_len = tcp_p->off << 2;
		uint32_t http_len = ntohs(ip_p->len) - ip_len - tcp_len;
		uint8_t *http_p = (uint8_t*)tcp_p + tcp_len;
		if (http_len) {
			string payload(http_p, http_p + http_len);
			if (strncmp(payload.c_str(), "GET", 3)) continue;

			smatch m;
			if (regex_search(payload, m, pattern)) {
				string URL = m[1].str();
				uint128_t url_md5;
				uint16_t key = make_md5(URL, &url_md5, TABLESIZE, true);

				if (binarysearch(value[key], url_md5, value[key].size())) {
					ip_p->len = htons(ntohs(ip_p->len) - http_len);
					tcp_p->flag = RST + ACK;
					WinDivertHelperCalcChecksums(packet, ntohs(ip_p->len), &addr, 0);
					if (!WinDivertSend(handle, packet, ntohs(ip_p->len), &addr, NULL)) {
						fprintf(stderr, "warning: failed to send\n");
						continue;
					}

					pkt.ip.len = htons(pkt_len);
					pkt.ip.src = ip_p->dst;
					pkt.ip.dst = ip_p->src;
					pkt.tcp.s_port = tcp_p->d_port;
					pkt.tcp.d_port = tcp_p->s_port;
					pkt.tcp.seq = tcp_p->ack;
					pkt.tcp.ack = htonl(ntohl(tcp_p->seq) + http_len);
					pkt.tcp.flag = FIN + ACK;
					WinDivertHelperCalcChecksums(&pkt, pkt_len, &addr, 0);
					if (!WinDivertSend(handle, &pkt, pkt_len, &addr, NULL)) {
						fprintf(stderr, "warning: failed to send\n");
						continue;
					}
					printf("Blocked URL : %s\n", URL.c_str());
				}
			}
		}
	}
}

int main() {
	// Use sorted url_md5
	if (load_txt(FILENAME)) { fprintf(stderr, "error: program exit\n"); return 1; }

	uint16_t priority = 0;
	HANDLE handle = WinDivertOpen("true", WINDIVERT_LAYER_NETWORK, priority, WINDIVERT_FLAG_SNIFF);
	if (handle == INVALID_HANDLE_VALUE) {
		fprintf(stderr, "error: failed to open the WinDivert device\n");
		return 1;
	}
	puts("[ Filtering Start! ]");
	filter(handle);
	return 0;
}
