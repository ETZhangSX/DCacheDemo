#include <iostream>
#include <ctime>
#include <map>
#include "servant/Communicator.h"
#include "servant/ServantProxy.h"
#include "Proxy.h"

using namespace std;
using namespace tars;

#define START "[\033[32mSTART\033[0m]"
#define END "[ \033[31mEND\033[0m ]"
#define SUBTEST_PREFIX "\t-- "

// 应用 Proxy 对象名
static string DCacheTestDemoObj = "DCache.TestDemoProxyServer.ProxyObj";

// 缓存模块名
static string ModuleTestDemoKV    = "TestDemoKV";
static string ModuleTestDemoKKRow = "TestDemoKKRow";
static string ModuleTestDemoList  = "TestDemoList";
static string ModuleTestDemoSet   = "TestDemoSet";
static string ModuleTestDemoZSet  = "TestDemoZSet";

// 测试
void testKV(DCache::ProxyPrx prx);
void testKKRow(DCache::ProxyPrx prx);
void testList(DCache::ProxyPrx prx);
void testSet(DCache::ProxyPrx prx);
void testZSet(DCache::ProxyPrx prx);

// KV 读写测试
void testSetKV(const string &key, const string &value, DCache::ProxyPrx prx);
void testGetKV(const string &key, DCache::ProxyPrx prx);

// KKRow 读写测试
void testInsertMKV(const string &mainKey, const map<string, string> &data, DCache::ProxyPrx prx);
void testGetMKV(const string &mainKey, DCache::ProxyPrx prx);

// List 读写测试
void testPushList(const string &mainKey, const vector<map<string, string>> &data, DCache::ProxyPrx prx);
void testGetList(const string &mainKey, const long &index, DCache::ProxyPrx prx);

// Set 读写测试
void testAddSet(const string &mainKey, const map<string, string> &data, DCache::ProxyPrx prx);
void testGetSet(const string &mainKey, DCache::ProxyPrx prx);

// ZSet 读写测试
void testAddZSet(const string &mainKey, const map<string, string> &data, const double &score, DCache::ProxyPrx prx);
void testGetZSet(const string &mainKey, DCache::ProxyPrx prx);

// 生成更新数据结构 UpdateValue
DCache::UpdateValue genUpdateValue(DCache::Op op, const string &value);

// 打印 map<string, string> 类型数据
void printMapData(const map<string, string> &data);

// 打印 vector<map<string,string> 类型数据
void printVectorMapData(const vector<map<string, string>> &data);

/************************************** main ************************************/
int main(int argc, char *argv[])
{
    CommunicatorPtr comm = new Communicator();
    try
    {
        TC_Config conf;
        // 解析配置文件
        conf.parseFile("config.conf");
        // 加载配置
        comm->setProperty(conf);
        // 生成代理
        auto prx = comm->stringToProxy<DCache::ProxyPrx>(DCacheTestDemoObj);

        testKV(prx);
        testKKRow(prx);
        testList(prx);
        testSet(prx);
        testZSet(prx);
    }
    catch (exception &e)
    {
        cerr << "error: " << e.what() << endl;
    }
    catch (...)
    {
        cerr << "Unknown Error" << endl;
    }
}

/******************************************************************************/

void testKV(DCache::ProxyPrx prx)
{
    cout << START << " testKV" << endl;

    testSetKV("hello", "world", prx);
    testGetKV("hello", prx);

    cout << END << " testKV" << endl;
}

void testKKRow(DCache::ProxyPrx prx)
{
    cout << START << " testKKRow" << endl;

    string mainKey = "Key";
    map<string, string> data1, data2;
    data1["UID"] = "test1";
    data1["VALUE"] = "hello";
    data2["UID"] = "test2";
    data2["VALUE"] = "hey";

    testInsertMKV(mainKey, data1, prx);
    testInsertMKV(mainKey, data2, prx);

    testGetMKV(mainKey, prx);

    cout << END << " testKKRow" << endl;
}

void testList(DCache::ProxyPrx prx)
{
    cout << START << " testList" << endl;

    string mainKey = "test";
    vector<map<string, string>> data;
    map<string, string> item;
    item["VALUE"] = "test";
    data.push_back(item);

    testPushList(mainKey, data, prx);

    long index = 0;
    testGetList(mainKey, index, prx);

    cout << END << " testList" << endl;
}

void testSet(DCache::ProxyPrx prx)
{
    cout << START << " testSet" << endl;

    string mainKey = "testKey";
    map<string, string> data1, data2, data3, data4;
    data1["VALUE"] = "hello";
    data2["VALUE"] = "hello";
    data3["VALUE"] = "hi";
    data4["VALUE"] = "test";

    testAddSet(mainKey, data1, prx);
    testAddSet(mainKey, data2, prx);
    testAddSet(mainKey, data3, prx);
    testAddSet(mainKey, data4, prx);

    testGetSet(mainKey, prx);

    cout << END << " testSet" << endl;
}

void testZSet(DCache::ProxyPrx prx)
{
    cout << START << " testZSet" << endl;

    string mainKey = "testKey";
    map<string, string> data1, data2, data3, data4;
    double score1, score2, score3, score4;
    data1["VALUE"] = "hello";
    score1 = 0.1;
    data2["VALUE"] = "hello";
    score2 = 0.1;
    data3["VALUE"] = "hi";
    score3 = 0.8;
    data4["VALUE"] = "test";
    score4 = 0.5;

    testAddZSet(mainKey, data1, score1, prx);
    testAddZSet(mainKey, data2, score2, prx);
    testAddZSet(mainKey, data3, score3, prx);
    testAddZSet(mainKey, data4, score4, prx);

    testGetZSet(mainKey, prx);

    cout << END << " testZSet" << endl;
}

DCache::UpdateValue genUpdateValue(DCache::Op op, const string &value)
{
    DCache::UpdateValue updateValue;
    updateValue.op = op;
    updateValue.value = value;
    return updateValue;
}

// 打印 map 类型数据
void printMapData(const map<string, string> &data)
{
    map<string, string>::const_iterator it = data.begin();
    while (it != data.end())
    {
        cout << "|" << it->first << ":" << it->second;
        ++it;
    }
    cout << endl;
}

// 打印 vector<map> 数据
void printVectorMapData(const vector<map<string, string>> &data)
{
    for (auto item : data)
    {
        cout << '\t' << SUBTEST_PREFIX;
        printMapData(item);
    }
}

void testSetKV(const string &key, const string &value, DCache::ProxyPrx prx)
{
    cout << SUBTEST_PREFIX << "setKV |" << key << ":" << value << endl;
    // 构造键值对
    DCache::SSetKeyValue setReq;
    setReq.keyItem = key;
    setReq.value = value;

    // 构造请求
    DCache::SetKVReq setRsp;
    setRsp.moduleName = ModuleTestDemoKV;
    setRsp.data = setReq;

    prx->setKV(setRsp);
}

void testGetKV(const string &key, DCache::ProxyPrx prx)
{
    // 构建请求
    DCache::GetKVReq getReq;
    getReq.moduleName = ModuleTestDemoKV;
    getReq.keyItem = key;

    // 返回数据结构体
    DCache::GetKVRsp getRsp;

    prx->getKV(getReq, getRsp);
    cout << SUBTEST_PREFIX << "getKV |" << key  << ":" << getRsp.value << "|" << "@ExpireTime:" << getRsp.expireTime << endl;
}

void testInsertMKV(const string &mainKey, const map<string, string> &data, DCache::ProxyPrx prx)
{
    cout << SUBTEST_PREFIX << "insertMKV ";
    printMapData(data);

    // 构造插入数据
    DCache::InsertKeyValue insertData;
    insertData.mainKey = mainKey;

    map<string, string>::const_iterator it = data.begin();
    while (it != data.end())
    {
        insertData.mpValue[it->first] = genUpdateValue(DCache::SET, it->second);
        ++it;
    }

    // 构造请求
    DCache::InsertMKVReq insertReq;
    insertReq.moduleName = ModuleTestDemoKKRow;
    insertReq.data = insertData;

    prx->insertMKV(insertReq);
}

void testGetMKV(const string &key, DCache::ProxyPrx prx)
{
    cout << SUBTEST_PREFIX << "getMKV    " << endl;

    // 构造请求
    DCache::GetMKVReq req;
    req.moduleName = ModuleTestDemoKKRow;
    req.mainKey = key;
    req.field = "*";

    DCache::GetMKVRsp rsp;
    prx->getMKV(req, rsp);

    printVectorMapData(rsp.data);
}

void testPushList(const string &mainKey, const vector<map<string, string>> &data, DCache::ProxyPrx prx)
{
    cout << SUBTEST_PREFIX << "pushList ";
    // 构造请求
    DCache::PushListReq req;
    req.moduleName = ModuleTestDemoList;
    req.mainKey = mainKey;
    // req.atHead = false;

    for (auto item : data)
    {
        DCache::InsertKeyValue insertValue;
        insertValue.mainKey = mainKey;
        insertValue.expireTimeSecond = 60 * 60 * 24;

        map<string, string>::const_iterator it = item.begin();
        while (it != item.end())
        {
            insertValue.mpValue[it->first] = genUpdateValue(DCache::SET, it->second);
            ++it;
        }
        req.data.push_back(insertValue);
    }

    int ret = prx->pushList(req);

    if (ret == DCache::ET_SUCC)
    {
        printVectorMapData(data);
        return;
    }
    cout << "ret: " << ret << endl;
}

void testGetList(const string &mainKey, const long &index, DCache::ProxyPrx prx)
{
    cout << SUBTEST_PREFIX << "getList  ";

    DCache::GetListReq req;
    req.moduleName = ModuleTestDemoList;
    req.mainKey = mainKey;
    req.field = "*";
    req.index = index;

    DCache::GetListRsp rsp;
    int ret = prx->getList(req, rsp);

    if (ret == DCache::ET_SUCC)
    {
        printMapData(rsp.entry.data);
        return;
    }
    cout << "ret:" << ret << endl;
}

void testAddSet(const string &mainKey, const map<string, string> &data, DCache::ProxyPrx prx)
{
    cout << SUBTEST_PREFIX << "addSet ";

    // 构造请求
    DCache::AddSetReq req;
    req.moduleName = ModuleTestDemoSet;
    req.value.mainKey = mainKey;
    req.value.expireTime = time(NULL) + 60 * 60 * 24;

    map<string, string>::const_iterator it = data.begin();
    while (it != data.end())
    {
        req.value.data[it->first] = genUpdateValue(DCache::SET, it->second);
        ++it;
    }

    int ret = prx->addSet(req);

    if (ret == DCache::ET_SUCC)
    {
        printMapData(data);
        return;
    }
    cout << "ret:" << ret << endl;
}

void testGetSet(const string &mainKey, DCache::ProxyPrx prx)
{
    cout << SUBTEST_PREFIX << "getSet " << endl;

    // 构造请求
    DCache::GetSetReq req;
    req.moduleName = ModuleTestDemoSet;
    req.mainKey = mainKey;
    req.field = "*";

    DCache::BatchEntry rsp;
    int ret = prx->getSet(req, rsp);

    if (ret == DCache::ET_SUCC)
    {
        // 打印返回值
        printVectorMapData(rsp.entries);
        return;
    }
    cout << "ret:" << ret << endl;
}

void testAddZSet(const string &mainKey, const map<string, string> &data, const double &score, DCache::ProxyPrx prx)
{
    cout << SUBTEST_PREFIX << "addZSet ";

    // 构造请求
    DCache::AddZSetReq req;
    req.moduleName = ModuleTestDemoZSet;
    req.value.mainKey = mainKey;
    req.score = score;

    map<string, string>::const_iterator it = data.begin();
    while (it != data.end())
    {
        req.value.data[it->first] = genUpdateValue(DCache::SET, it->second);
        ++it;
    }

    int ret = prx->addZSet(req);

    if (ret == DCache::ET_SUCC)
    {
        printMapData(data);
        return;
    }
    cout << "ret:" << ret << endl;
}

void testGetZSet(const string &mainKey, DCache::ProxyPrx prx)
{
    cout << SUBTEST_PREFIX << "getZSet " << endl;

    // 构造请求
    DCache::GetZsetByPosReq req;
    req.moduleName = ModuleTestDemoZSet;
    req.mainKey = mainKey;
    req.field = "*";
    req.start = 0;
    req.end = 3;

    DCache::BatchEntry rsp;
    int ret = prx->getZSetByPos(req, rsp);

    if (ret == DCache::ET_SUCC)
    {
        // 打印返回值
        printVectorMapData(rsp.entries);
        return;
    }
    cout << "ret:" << ret << endl;
}