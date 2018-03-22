#pragma once
/**
* @brief 入力イベントクラス
* @auhter leesatoh
*/
#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include "interface.h"
namespace asio = boost::asio;
namespace ip = boost::asio::ip;
/**
* @fn
* 入力イベント
* 主にソケット
* 必須
*  ioservice
*/
struct  HEADER{
    char    MsgLength           [ 4];   //電文長(ヘッダー部を除く電文長atoi()で変換可能な値
    char    MsgKind             [ 1];   //電文種別(
    char    SerialNo            [ 8];   //シリアル番号
    char    MsgCreateDateTime   [26];   //電文作成日時(GWが電文を作成した時刻yyyy/mm/dd hh24:mi:ss.ssssss※.sssssss部分はOSに依存
};
class CInputEventEcn :IInputEvent{
	asio::streambuf readbuf_;
	ip::tcp::socket sock;
	public:
		CInputEventEcn(asio::io_service& ios, const std::string& addr, int port):sock(ios){
			sock.connect(ip::tcp::endpoint(ip::address::from_string(addr), port));
			AsyncRead();
		}
		~CInputEventEcn(){
			Close();
		}
		void Close(){
			ShutdownRead();
			boost::system::error_code ec;
			sock.close(ec);
			if(ec){
				std::cout<<ec<<std::endl;
			}else{
				std::cout << "Close OK" << std::endl;
			}
		}
	private:
		void AsyncRead()
			{
				asio::async_read(sock, readbuf_, asio::transfer_at_least(sizeof(HEADER)), boost::bind(&CInputEventEcn::OnAsyncRead, this, asio::placeholders::error));
			}
		void ShutdownRead()
			{
				if(!sock.is_open()){	return;			}
				sock.shutdown(boost::asio::ip::tcp::socket::shutdown_receive);
				if(sock.available()){	return;		}
				boost::system::error_code errcode;
				asio::read(sock, readbuf_, errcode);
				if(errcode && errcode != boost::asio::error::eof)
				{
					std::cout << errcode << std::endl;
				}else{
					ReadBufConsume();
				}
			}
		void OnAsyncRead(const boost::system::error_code& errcode)
			{
				if(errcode && errcode != boost::asio::error::eof)
				{
					std::cout << errcode << std::endl;
				}else{
					ReadBufConsume();
					if(!errcode){
						AsyncRead();
					}
				}
			}
			
		void ReadBufConsume(){
				size_t			len;
				size_t			idx;
				int				msglen;


				while(true){
					const std::string buf = asio::buffer_cast<const char*>(readbuf_.data());
					len = readbuf_.size();
					if(len < sizeof(HEADER)) break;
					msglen = std::stoi(buf, &idx, 10);	//10進数
					if(len < (size_t)msglen)	break;
					//外部処理
					std::cout << " msglen:"<< msglen << std::endl;
					
					readbuf_.consume(msglen);
				}
			}
};
