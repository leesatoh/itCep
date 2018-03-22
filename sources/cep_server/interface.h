#pragma once
/**
* @brief インターフェースのみ
* @auhter leesatoh
*/
#include <boost/system/error_code.hpp>
/**
* @fn
* 入力イベント
* 主にソケット
* 必須
*  ioservice
*/
class IInputEvent
{
	public:
		virtual void Close()=0;
	private:
		virtual void AsyncRead()=0;
		virtual void OnAsyncRead(const boost::system::error_code& error)=0;
		virtual void ShutdownRead()=0;
		virtual void ReadBufConsume()=0;
};
