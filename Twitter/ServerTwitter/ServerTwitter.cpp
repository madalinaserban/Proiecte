#include <iostream>
#include "Database.h"
#include "SingleTon.h"
#include "../Logger//Logger.h"
#include "SocialGraph.h"
#include "ServiceSrv.h"
#include "Posts.h"
#include "../Network/TcpSocket.h"

#include "UtilityFunctions.h"
int main()
{
	std::shared_ptr<Logger> logger = SingleTon<Logger>::GetInstance();
	logger->WriteLog("[Server] Server opened", Logger::Level::INFO);
	std::shared_ptr<Database> db = SingleTon<Database>::GetInstance();
	std::shared_ptr<Posts> posts = SingleTon<Posts>::GetInstance();
	std::shared_ptr<SocialGraph> socialGraph = SingleTon<SocialGraph>::GetInstance();
	std::unique_ptr<ServiceSrv> service;
	service->ListenFromClient();
	logger->WriteLog("[Server] Server closed", Logger::Level::INFO);
}