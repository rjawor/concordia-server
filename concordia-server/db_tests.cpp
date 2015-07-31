#include <libpq-fe.h>

static void
exit_nicely(PGconn *conn)
{
    PQfinish(conn);
    throw ConcordiaException("Closing connection and shutting down");
}


            const char *conninfo;
            PGconn     *conn;
            PGresult   *res;
            int         nFields;
            int         i,
                        j;
            
            conninfo = "dbname=concordia_server user=concordia";

            /* Make a connection to the database */
            conn = PQconnectdb(conninfo);

            /* Check to see that the backend connection was successfully made */
            if (PQstatus(conn) != CONNECTION_OK)
            {
                fprintf(stderr, "Connection to database failed: %s",
                        PQerrorMessage(conn));
                exit_nicely(conn);
            }

            /*
             * Our test case here involves using a cursor, for which we must be inside
             * a transaction block.  We could do the whole thing with a single
             * PQexec() of "select * from pg_database", but that's too trivial to make
             * a good example.
             */

            /* Start a transaction block */
            res = PQexec(conn, "BEGIN");
            if (PQresultStatus(res) != PGRES_COMMAND_OK)
            {
                fprintf(stderr, "BEGIN command failed: %s", PQerrorMessage(conn));
                PQclear(res);
                exit_nicely(conn);
            }

            /*
             * Should PQclear PGresult whenever it is no longer needed to avoid memory
             * leaks
             */
            PQclear(res);

            /*
             * Fetch rows from pg_database, the system catalog of databases
             */
            res = PQexec(conn, "SELECT * FROM language");
            if (PQresultStatus(res) != PGRES_TUPLES_OK)
            {
                fprintf(stderr, "SELECT failed: %s", PQerrorMessage(conn));
                PQclear(res);
                exit_nicely(conn);
            }

            jsonWriter.String("attributes");
            jsonWriter.StartArray();
            /* first, print out the attribute names */
            nFields = PQnfields(res);
            for (i = 0; i < nFields; i++)
                jsonWriter.String(PQfname(res, i));

            jsonWriter.EndArray();


            jsonWriter.String("values");
            jsonWriter.StartArray();
            /* next, print out the rows */
            for (i = 0; i < PQntuples(res); i++)
            {
                jsonWriter.StartArray();
                for (j = 0; j < nFields; j++)
                    jsonWriter.String(PQgetvalue(res, i, j));
                jsonWriter.EndArray();
            }

            jsonWriter.EndArray();

            PQclear(res);

            /* end the transaction */
            res = PQexec(conn, "END");
            PQclear(res);

            /* close the connection to the database and cleanup */
            PQfinish(conn);
            

