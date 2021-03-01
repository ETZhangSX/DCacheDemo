# DCacheDemo

DCache 使用示例，包含了 `key-value`, `k-k-row`, `list`, `set`, `zset` 的使用示例。

具体使用方式可以查阅 `main.cpp` 中的使用方式。

## 环境依赖

运行本 Demo 需要

- 提前部署好 TARS 和 DCahce，相关安装部署文档可以参考 [TARS 框架部署](https://tarscloud.github.io/TarsDocs/SUMMARY.html#deploy)和 [DCache 安装文档](https://github.com/Tencent/DCache/blob/master/docs/install.md)；
- 安装 TarsCpp 开发环境，参考 [TarsCpp 开发环境部署](https://tarscloud.github.io/TarsDocs/env/tarscpp.html)；如果 Demo 在安装 TARS 的机器或虚拟机上运行，则可以忽略这一步，TARS 中已经预装 TarsCpp；
- 确保运行的环境和 TARS、DCache 部署的机器网络互通；
- 修改 `main.cpp` 中 DCache 应用和模块的名称为自己创建部署的模块名称
    ```cpp
    // 应用 Proxy 对象名
    static string DCacheTestDemoObj = "DCache.TestDemoProxyServer.ProxyObj";

    // 缓存模块名
    static string ModuleTestDemoKV    = "TestDemoKV";
    static string ModuleTestDemoKKRow = "TestDemoKKRow";
    static string ModuleTestDemoList  = "TestDemoList";
    static string ModuleTestDemoSet   = "TestDemoSet";
    static string ModuleTestDemoZSet  = "TestDemoZSet";
    ```
- 修改配置文件 `config.conf`，修改主控 IP 为你自己 TARS 部署的机器 IP，配置文件内容如下

```xml
<tars>
  <application>
    <client>
        #tarsregistry locator
        locator = tars.tarsregistry.QueryObj@tcp -h 192.168.1.123 -t 60000 -p 17890
    </client>
  </application>
</tars>

```

## 运行

运行以下命令进行编译构建

```sh
make
./DCacheClient
```