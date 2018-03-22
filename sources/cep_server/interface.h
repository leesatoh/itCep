#pragma once
/**
* @brief �C���^�[�t�F�[�X�̂�
* @auhter leesatoh
*/
#include <boost/system/error_code.hpp>
/**
* @fn
* ���̓C�x���g
* ��Ƀ\�P�b�g
* �K�{
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
