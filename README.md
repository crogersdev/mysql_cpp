# mysql_cpp

step 0: install packages...

```$ yum list installed | grep mysql
mysql-community-client.x86_64              5.6.46-2.el7                @mysql56-community
mysql-community-common.x86_64              5.6.46-2.el7                @mysql56-community
mysql-community-libs.x86_64                5.6.46-2.el7                @mysql56-community
mysql-community-release.noarch             el7-5                       installed
mysql-community-server.x86_64              5.6.46-2.el7                @mysql56-community
mysql-connector-c++.x86_64                 8.0.18-1.el7                @mysql-connectors-community
mysql-connector-c++-debuginfo.x86_64       8.0.18-1.el7                @mysql-connectors-community
mysql-connector-c++-devel.x86_64           8.0.18-1.el7                @mysql-connectors-community
mysql-connector-c++-jdbc.x86_64            8.0.18-1.el7                @mysql-connectors-community
mysql-connector-java.noarch                1:5.1.25-3.el7              @base
mysql-connector-odbc.x86_64                8.0.18-1.el7                @mysql-connectors-community
mysql-connector-odbc-debuginfo.x86_64      8.0.18-1.el7                @mysql-connectors-community
mysql-connector-odbc-setup.x86_64          8.0.18-1.el7                @mysql-connectors-community
mysql-connector-python.x86_64              8.0.18-1.el7                @mysql-connectors-community
mysql-connector-python-cext.x86_64         8.0.18-1.el7                @mysql-connectors-community
mysql-connector-python-debuginfo.x86_64    2.1.7-1.el7                 @mysql-connectors-community
```

step 1a: go find all the *.h files necessary for the `-I` flag in the compile part of the `g++` call

```
$ cd /usr/include
$ find . -name '*mysql*h'
./mysql-cppconn-8/jdbc/mysql_connection.h
./mysql-cppconn-8/jdbc/mysql_driver.h
./mysql-cppconn-8/jdbc/mysql_error.h
./mysql-cppconn-8/mysqlx/devapi/mysql_charsets.h
./mysql-cppconn-8/mysqlx/devapi/mysql_collations.h
```

ok, fantastic.  since our find regex won't show us EVERYTHING in that `mysql-cppconn-8` dir, we can go sniffing around to find which files look like good candidates to include in the top of the source file.  that's what i did... include a file, compile, see if the symbols are found, if not try again...

so i wound up going with `-I /usr/include/mysql-cppconn-8/jdbc` so i could do `#include <some_file.h>` and `#include <cppconn/some_other_file.h>`

step 1b: go find all the the *.so file and its associated library(ies) necessary for `-l` flags for the linker part of the `g++` call
```
$ cd /usr
$ find . -name '*mysql*so'
./lib64/python2.7/site-packages/_mysqlxpb.so
./lib64/python2.7/site-packages/_mysql_connector.so
./lib64/mysql/plugin/debug/mysql_no_login.so
./lib64/mysql/plugin/mysql_no_login.so
./lib64/libmysqlcppconn.so
./lib64/libmysqlcppconn8.so
find: ‘./share/polkit-1/rules.d’: Permission denied
find: ‘./libexec/initscripts/legacy-actions/auditd’: Permission denied
```

^^^ note that `/usr/lib64/libmysqlcppconn.so` is a symlink to the precise version, `/usr/lib64/libmysqlcppconn.so`

anyway, that'll be our `-l` arg, minus the `lib` part.  not sure of the relationship of names between the *.so file and the associated library...  something about `sudo ldconfig -p` and idk, google the rest.  i'd like to know so if you find out let me know, please.

step 2: compile, link, build, and run

```
$ g++ -I /usr/include/mysql-cppconn/jdbc -l mysqlcppconn -o mysql mysql.cpp -Wall -std=c++11
$ ./mysql
$ ./mysql

Running 'SELECT 'Hello World!' AS _message'...
        ... MySQL replies: Hello World!
        ... MySQL says it again: Hello World!
```
