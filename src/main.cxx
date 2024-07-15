#include <drogon/drogon.h>



int main (int argc, char* argv[]){

    char* db_host;
    char* server_host;
    if (argc <3){
        db_host = "127.0.0.1";
        server_host= "127.0.0.1";
    } else {
        server_host = argv[1];
        db_host = argv[2];
    }
    auto &app = drogon::app();
    //App run
    LOG_INFO << "Server start on "<< server_host<<":8080";
    app.setLogLevel(trantor::Logger::kDebug)
        .createDbClient("postgresql",db_host,5433,"cxx_test","postgres","")
        .addListener(server_host,8080)
        .setThreadNum(4)
        .run();


}