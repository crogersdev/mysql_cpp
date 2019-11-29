/* Standard C++ includes */
#include <stdlib.h>
#include <iostream>
#include <vector>

/*
  Include directly the different
  headers from cppconn/ and mysql_driver.h + mysql_util.h
  (and mysql_connection.h). This will reduce your build time!
*/

#include <mysql_connection.h>
#include <mysql_driver.h>
#include <cppconn/connection.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>

using namespace std;

int main(void)
{
  cout << endl;

  try {
    sql::Driver *pDriver; // can't make this guy shared/unique becaues the destructor is protected.

    /* Create a connection */
    pDriver = get_driver_instance();
    unique_ptr<sql::Connection> pCon (pDriver->connect("tcp://127.0.0.1:3306", "root", ""));
    /* Connect to the MySQL test database */
    // for some reason, 'databases' are almost synonymous with 'schemas.'
    pCon->setSchema("test");

    unique_ptr<sql::Statement> pStmt(pCon->createStatement());
    unique_ptr<sql::ResultSet> pResultSet(pStmt->executeQuery("select * from cus_tbl"));
    sql::ResultSetMetaData *pRsmd = pResultSet->getMetaData();
    
    // let's take a look at columns
    vector<string> colNames = {};
    cout << "the columns are..." << std::endl;
    for (uint32_t i = 1; i <= pRsmd->getColumnCount(); i++)
    {
      string colName = pRsmd->getColumnName(i);
      cout << colName << "\t";
      colNames.push_back(colName);
    }

    cout << endl;

    cout << "--------------------------------------------" << endl;

    // there's gotta be away to determine the type of the column
    // and then automagically know which getThing() method to call.
    while (pResultSet->next())
    {
      for (const auto& colName: colNames)
      {
        cout << pResultSet->getString(colName) << "\t";
      }
      cout << endl;
    }

  } catch (sql::SQLException &e) {
    cout << "# ERR: SQLException in " << __FILE__;
    cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
    cout << "# ERR: " << e.what();
    cout << " (MySQL error code: " << e.getErrorCode();
    cout << ", SQLState: " << e.getSQLState() << " )" << endl;
  }

  cout << endl;

  return EXIT_SUCCESS;
}
