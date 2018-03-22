#include "CInputEvent.hpp"
/**
* @brief ���C��
* @auhter leesatoh
*/
//�V�O�i��
class Signal{
	asio::signal_set signal_set;
	public:
		Signal(asio::io_service& ios)
		:signal_set(ios, SIGINT)
		{
			signal_set.async_wait(boost::bind(&Signal::OnCancel, this, asio::placeholders::error, boost::asio::placeholders::signal_number));
		}
	private:
		void OnCancel(const boost::system::error_code& error, int signal_number)
		{
		    if (!error && signal_number == SIGINT)
		      std::cout << "on cancel signal" << std::endl;
		   signal_set.get_io_service().stop();
		}
};

//���C��
int main()
{
	//on_cancel�̂Ƃ��Ƀ\�P�b�g���N���[�Y������
    asio::io_service ios;

    
	Signal sg(ios);
    CInputEventEcn cl(ios, "192.168.59.208", 16101);
    ios.run();
    cl.Close();
}
