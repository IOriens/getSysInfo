#include <dirent.h>
#include <pwd.h>  
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/utsname.h> //uname
#include <unistd.h>


//定义一个cpu occupy的结构体
typedef struct PACKED {
    char name[20];      //定义一个char类型的数组名name有20个元素
    unsigned int user; //定义一个无符号的int类型的user
    unsigned int nice; //定义一个无符号的int类型的nice
    unsigned int system;//定义一个无符号的int类型的system
    unsigned int idle; //定义一个无符号的int类型的idle
} CPU_OCCUPY;

// CPU 信息获取
void get_cpuoccupy (CPU_OCCUPY *cpust) //对无类型get函数含有一个形参结构体类弄的指针O
{   
    FILE *fd;         
    int n;            
    char buff[256]; 
    CPU_OCCUPY *cpu_occupy;
    cpu_occupy=cpust;

    fd = fopen ("/proc/stat", "r"); 
    fgets (buff, sizeof(buff), fd);
    
    sscanf (buff, "%s %u %u %u %u", cpu_occupy->name, &cpu_occupy->user, &cpu_occupy->nice,&cpu_occupy->system, &cpu_occupy->idle);
    
    fclose(fd);     

    // printf("CPU Usage: %s", buff);
}

// 内存信息获取
int getMemoryInfo() {
    char buff[256];
    FILE* fd;
    int nTotal;
    int nUsed;
    char  *pLine;

    if(system("free -m | awk '{print $2,$3}' > out/mem2")) {

    }

    memset(buff, 0, 256);

    if(fd = fopen("out/mem2", "rb"))    {
        // return -1;
    }

    fread(buff,1, sizeof(buff) , fd);
    pLine = strstr(buff, "\n");
    nTotal= atoi(pLine);
    pLine= strstr(pLine, " ");
    nUsed= atoi(pLine);
    memset(buff, 0, 256);
    sprintf(buff, "Used %dM     /    Total %dM\n", nUsed, nTotal);
  //   if(strlen(buff) > len)    {
  //     return -1;
  // }
    printf("Memory Usage: %s", buff);
    fclose(fd);
}

// 目录信息获取
void getDirInfo() {
    // http://nayan115.blog.sohu.com/231443440.html    
    // 文件信息
    DIR *dir;
    struct dirent *ptr;
    char path[100] = "/home";

    // Print all dir
    printf("%s 路径下文件夹:\n", path);
    dir = opendir(path);
    while((ptr = readdir(dir)) != NULL) {

        if ( !strcmp(ptr->d_name, ".") || !strcmp(ptr->d_name, "..") ) {
            // do nothing (straight logic)
        } else {    
            if(ptr->d_type == 4) {
                // Directory
                printf("++++: %s\n", ptr->d_name);
                // printf("d_type: %i\n", ptr->d_type); 
            } else {
                // File
                // printf("----: %s\n", ptr->d_name);
            }

        }

    }
    closedir(dir);

    // Print all files
    dir = opendir(path);
    printf("%s 路径下文件:\n", path);
    while((ptr = readdir(dir)) != NULL) {

        if ( !strcmp(ptr->d_name, ".") || !strcmp(ptr->d_name, "..") ) {
            // do nothing (straight logic)
        } else {    
            if(ptr->d_type == 4) {
                // Directory
                // printf("++++: %s\n", ptr->d_name);
                // printf("d_type: %i\n", ptr->d_type); 
            } else {
                // File
                printf("----: %s\n", ptr->d_name);
            }

        }

    }
    closedir(dir);
}

// 进程信息获取
void getPidInfo() {
     pid_t my_pid, parent_pid;  
     uid_t my_uid, parent_uid;  
     gid_t my_gid, parent_gid;  

     struct passwd *my_info;  

     my_pid      = getpid();  
     parent_pid  = getppid();  
     my_uid      = getuid();  
     parent_uid  = geteuid();  
     my_gid      = getgid();  
     parent_gid  = getegid();  

     my_info     = getpwuid(my_uid);  

     printf("\n              进程信息          \n");  
     printf("===================================\n");  


     printf("Process ID : %ld\n", (long)my_pid);  
     printf("Parent  ID : %ld\n", (long)parent_pid);  
     printf("User    ID : %ld\n", (long)my_uid);  
     printf("Effective User  ID : %ld\n", (long)parent_uid);  
     printf("Group   ID : %ld\n", (long)my_gid);  
     printf("Effective Group ID : %ld\n", (long)parent_gid);  


     if(my_info){  
        printf("\n              用户信息          \n");  
        printf("===================================\n"); 


        printf("My Login Name : %s\n", my_info->pw_name);  
        printf("My Password   : %s\n", my_info->pw_passwd);      
        printf("My User ID    : %ld\n", (long)my_info->pw_uid);  
        printf("My Group ID   : %ld\n", (long)my_info->pw_gid);  
        printf("My Real Name  : %s\n", my_info->pw_gecos);  
        printf("My Home Dir   : %s\n", my_info->pw_dir);  
        printf("My Work Shell : %s\n", my_info->pw_shell);    
    }
}

int main(int argc, char **argv[]) {

    printf("\n              系统信息          \n");  
    printf("===================================\n"); 

    struct utsname  u;
    if (uname(&u) != -1) {
        printf("获取当前内核的名称和信息如下\n"
            "sysname:%s\n"
            "nodename:%s\n"
            "release:%s\n"
            "version:%s\n"
            "machine:%s\n"
            , u.sysname, u.nodename, u.release, u.version, u.machine);
    }


    printf("\n              当前目录执行信息          \n");  
    printf("===================================\n");  

    char *name;
    char *path;

    name = getenv("LOGNAME");
    path = getenv("PWD");

    printf("logname = %s\n",name);
    printf("pwd = %s\n",path);
    printf("\n              文件信息          \n");  
    printf("===================================\n");  

    getDirInfo();

    printf("\n              内存信息          \n");  
    printf("===================================\n");  

    getMemoryInfo();


    printf("\n              CPU信息          \n");  
    printf("===================================\n");  

    // user: normal processes executing in user mode
    // nice: niced processes executing in user mode
    // system: processes executing in kernel mode
    // idle: twiddling thumbs
    CPU_OCCUPY cpu_stat1;
    get_cpuoccupy((CPU_OCCUPY *)&cpu_stat1);
    printf("CPU Usage: user = %i, nice, = %i, system = %i, idle = %i\n", cpu_stat1.user, cpu_stat1.nice, cpu_stat1.system, cpu_stat1.idle);

    // Network Info
    // getNetworkInfo()

    // PID info
    getPidInfo();

    return 0;
}