

In order to configure Concordia as upstart job, copy the 3 .conf files into your /etc/init and run:

sudo apt-get install upstart upstart-sysv
sudo initctl reload-configuration

Also, add to the file /etc/init.d/postgresql the line:

initctl emit -n started JOB=postgresql

at the end of postgresql start sequence, after the loop:

for v in $versions; do
           $1 $v || EXIT=$?
       done
