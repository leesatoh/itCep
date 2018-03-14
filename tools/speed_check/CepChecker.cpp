//

#include "ThreadPool.hpp"
#include "CepMessage.hpp"
#include "WorkSndRcv.hpp"


int main()
{
	char c;
	cout << "クライアント？サーバー？c/s\n>";
	cin >> c;
	if (c == 'c')
	{
		cWorkClient WorkClient("127.0.0.1", 20001);
		WorkClient.run();
	}
	else if (c == 's')
	{
		cCepMessage CepMessage;
		cThreadPool ThreadPool(4);
		ThreadPool.add(std::make_shared<cWorkServer>(20001, &CepMessage));
	}
	
    return 0;
}

