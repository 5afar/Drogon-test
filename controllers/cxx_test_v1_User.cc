#include "cxx_test_v1_User.h"
#include "../model/PersonalData.h"
#include "../model/Users.h"

using namespace cxx_test::v1;

void User::health(
    const HttpRequestPtr &req,
    std::function<void(const drogon::HttpResponsePtr &)> &&callback)
{
    auto resp = drogon::HttpResponse::newHttpResponse();
    resp->setContentTypeCode(drogon::CT_TEXT_PLAIN);
    resp->setBody("OK");
    callback(resp);
}

void User::getUsers(const HttpRequestPtr &req,
                    std::function<void(const drogon::HttpResponsePtr &)> &&callback)
{
    auto dbClient = drogon::app().getDbClient();
    auto userMapper = drogon::orm::Mapper<drogon_model::cxx_test::Users>(dbClient);

    auto allUsers = userMapper.findAll();
    auto arr = Json::Value();
    for (auto &user : allUsers)
    {
        arr.append(user.toJson());
        LOG_DEBUG << *user.getId() << " " << *user.getLogin();
    }
    callback(HttpResponse::newHttpJsonResponse(arr));
}

void User::getPID(const drogon::HttpRequestPtr &req,
                  std::function<void(const drogon::HttpResponsePtr &)> &&callback, int searchNum)
{
    auto dbClient = drogon::app().getDbClient();
    auto PIDMapper = drogon::orm::Mapper<drogon_model::cxx_test::PersonalData>(dbClient);

    auto target = PIDMapper.findByPrimaryKey(searchNum);
    auto pidJson = target.toJson();

    std::promise<void> userPromise;
    auto userFuture = userPromise.get_future();

    target.getUsers(dbClient, [&](const drogon_model::cxx_test::Users user)
                    {
        pidJson["user"] = user.toJson();
        userPromise.set_value(); }, [&](const drogon::orm::DrogonDbException &err)
                    { userPromise.set_exception(
                          std::make_exception_ptr(err.base())); });
    userFuture.wait();
    callback(HttpResponse::newHttpJsonResponse(pidJson));
}

void User::addNew(const drogon::HttpRequestPtr &req,
                  std::function<void(const drogon::HttpResponsePtr &)> &&callback)
{
    Json::Value personData;
    std::stringstream(req->bodyData()) >> personData;
    auto userReq = drogon_model::cxx_test::Users(personData);
    auto pidReq = drogon_model::cxx_test::PersonalData(personData);

    auto dbClient = drogon::app().getDbClient();
    auto userMapper = drogon::orm::Mapper<drogon_model::cxx_test::Users>(dbClient);
    auto pidMapper = drogon::orm::Mapper<drogon_model::cxx_test::PersonalData>(dbClient);

    std::promise<Json::Value> addPromise;
    std::promise<Json::Value> addPromise2;

    auto addFuture = addPromise.get_future();
    auto addFuture2 = addPromise2.get_future();

    userMapper.insert(userReq, [&](const drogon_model::cxx_test::Users &user)
                      { addPromise.set_value(user.toJson()); }, [&](const drogon::orm::DrogonDbException &err)
                      { addPromise.set_exception(std::make_exception_ptr(err.base())); });
    pidMapper.insert(pidReq, [&](const drogon_model::cxx_test::PersonalData &pid)
                     { addPromise2.set_value(pid.toJson()); }, [&](const drogon::orm::DrogonDbException &err)
                     { addPromise2.set_exception(std::make_exception_ptr(err.base())); });
    addFuture2.wait();
    addFuture.wait();
    callback(HttpResponse::newHttpJsonResponse(addFuture.get()));
}