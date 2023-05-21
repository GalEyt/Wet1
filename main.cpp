#include "StreamingDBa1.h"

int main()
{
    streaming_database sdb;
    sdb.add_user(1,false);
    sdb.add_user(2, true);
    sdb.add_user(3,false);
    sdb.add_user(4,false);
    sdb.add_user(5,false);
    sdb.remove_user(3);
    sdb.add_group(1);
    sdb.add_user_to_group(1,1);
    sdb.add_user_to_group(4,1);
    sdb.add_user_to_group(5,1);
    sdb.add_user_to_group(2,1);
    sdb.remove_user(5);
    sdb.remove_user(2);
    sdb.remove_user(4);
    sdb.remove_user(1);
    sdb.remove_group(1);

}