#pragma once

#include <drogon/HttpController.h>

using namespace drogon;

namespace cxx_test
{
  namespace v1
  {
    class User : public drogon::HttpController<User>
    {
    public:
      METHOD_LIST_BEGIN
      // use METHOD_ADD to add your custom processing function here;
      // METHOD_ADD(User::get, "/{2}/{1}", Get); // path is /cxx_test/v1/User/{arg2}/{arg1}
      // METHOD_ADD(User::your_method_name, "/{1}/{2}/list", Get); // path is /cxx_test/v1/User/{arg1}/{arg2}/list
      // ADD_METHOD_TO(User::your_method_name, "/absolute/path/{1}/{2}/list", Get); // path is /absolute/path/{arg1}/{arg2}/list
      ADD_METHOD_TO(User::health, "/health", Get);
      ADD_METHOD_TO(User::getUsers, "/users", Get);
      ADD_METHOD_TO(User::getPID, "/PID/{1}", Get);
      ADD_METHOD_TO(User::addNew, "/create",Post);

      METHOD_LIST_END

      void health(
        const HttpRequestPtr &req, 
        std::function<void(const HttpResponsePtr &)> &&callback);
      void getUsers(
        const drogon::HttpRequestPtr &req,
        std::function<void(const drogon::HttpResponsePtr &)> &&callback);
      void getPID(
        const drogon::HttpRequestPtr &req,
        std::function<void(const drogon::HttpResponsePtr &)> &&callback, int searchNum);
      void addNew(
        const drogon::HttpRequestPtr &req,
        std::function<void(const drogon::HttpResponsePtr &)> &&callback);
    };
  }
}
