# 序章：进程相关概念
### 1、程序和进程
+ 程序是文件，在磁盘中
+ 进程是活跃的程序，在内存中
	+ 同一个程序可以运行多个不同的进程
	+ 进程是独立的，每个进程都占用各自的虚拟地址空间
	+ 多个进程的用户区映射到多个物理地址空间，但是内核区只映射到单个物理地址空间，但是多个进程的内核区在虚拟地址空间中并不相同，此功功能是由MMU来实现
	+ 子进程的会使用和父进程相同的代码段
### 2、并发
+ 进程A，B，C ，一个CUP在同一时刻只能运行一个进程
+ 单道程序设计模型 ： A运行完，B运行，B运行完，C运行，C运行完，不是并发的
+ 多道程序设计模型 ： A B C全部运行，每个程序会把自己的计划任务分成多个片段，cpu使用自己的时间轮片，分配给单个进程（A），在运行一个片段后，时钟中断（收回），再分配给其他进程（B），最后A B C 执行完成。 因为cpu的计算速度很快，所以宏观上是并发的
### 3、MMU 
+ 位于cpu中
+ 完成虚拟地址和物理地址的映射
+ 调整cup对内存的访问级别 ： Linux中只有两种级别 ： 用户空间 内核空间
# 第一章：PCB和进程控制块和进程的环境变量
+ PCB位于内核区
+ PCB是一个结构体 类型：task_struct
### 1、PCB的成员
+ 进程id 类型：pid_t 实际上是个unsigned int
+ 进程状态：
	+ 可运行状态 正在运行或者等待被cup运行的进程
	+ 等待状态 当进程等待某个事件（Event）或某个资源时就会进入等待状态，会放弃cup资源，进入等带队列
		+ 可中断等待状态 等带信号 可以被信号唤醒 
		+ 不可中断等待状态  只能用特定的函数去主动唤醒
    + 暂停状态 暂停来接收某些特殊的处理 ，比如调试
    +  僵死状态 进程终止 但是父进程没有执行wait()系统调用，终止进程的信息没有回收
+ 进程切换时需要保存和恢复的cup寄存器的值
	+ 因为cup在切换处理不同的进程时，使用的是同一套寄存机堆，新的进程会覆盖掉旧的进程寄存器中的数据
+ 虚拟地址和物理地址的映射表
+ 描述终端信息 
+ 当前进程的工作目录
+ umask掩码
+ 文件描述符表
+ 和信号相关的信息
+ 用户id和组id
+ 会话和进程组
+ 进程可以使用的资源上限 
	+ ulimit -a 查看资源上限 
+ 详情： https://blog.csdn.net/xyouyubl/article/details/55809313 
### 2、进程的环境变量
+ 引入环境变量 extern char ** environ
+ 是个字符串数组 以NULL作为结尾
+ 位置在高于stack的起始位置
+  getenv() 获取环境变量
+  setenv() 设置/创建 环境变量
+  unsetenv() 删除一个环境变量
# 第二章：进程控制
### 1、进程创建
+ fork() 创建一个子进程 
	+ 子进程拥有和父进程相同的代码段
	+ fork()函数往下的代码父进程和子进程都会执行
	+ fork()只调用了一次，但是会分成两个进程，父进程的返回值会被设置为子进程的pid，子进程的返回值会被设置为0
+ bash通过fork()创建的当前进程
+ 每个进程的执行顺序都不确定，是由cup内核调度算法来决定，可以通过sleep()来控制
### 2、父子进程共享
+ 父子进程的相同之处：全局变量 .text 栈 堆 环境变量 用户id 宿主目录  进程工作目录 信号处理方式
+ 父子不同之处：进程id fork()返回值 父进程id 进程运行时间 闹钟（定时器） 未决信号集
+ 父子的进程共享 ： 文件描述符 mmap建立的映射区
+ 父子进程的相同之处遵从读时共享写时复制，如果对一块内存进行写操作，就会开辟新的区域，否则公用同一块内存
### 3、gdb调试父子进程
+ gdb只能跟踪一个进程 默认跟踪父进程
+ set follow-fork-mode child 命令 设置gdb在fork之后跟踪子进程
+ set follow-fork-mode parent 命令 设置追踪父进程
### 4、exec函数族
+ 执行另一个程序，不会创建新的进程
+ execlp 
	+ 第1个参数是可执行程序名（不需要路径）
	+ 第2个以后的参数为命令行参数，最后以NULL结尾
	+ execlp("ls", "ls", "-l",NULL);
+ execl 第一个参数是路径
	+ 可以加载任意可执行程序
+ execle 最后一个参数传环境变量表
+ execv 第二个参数传命令行参数数组  
+ execvp
+ execve
+ 
+ l 命令行参数，p 搜索file使用path变量 v 使用命令行参数数组 e 使用环境变量数组
+ 
+ exec函数族没有成功返回值，只有失败返回值，调用成功后不会执行后面的代码
# 第三章：回收子进程
+ 正常情况下，子进程结束，父进程会回收子进程占用的资源，
### 1、孤儿进程
+ 当父进程在子进程执行结束前执行结束，该子进程就变成孤儿进程，孤儿进程的父进程就会变成init进程，init被称为孤儿院
### 2、僵尸进程
+ 在子进程执行完成后，父进程没有回收子进程的资源，子进程会留下PCB，子进程就变成一个僵尸进程
+ 父进程结束后会回收僵尸进程
+ kill不能清除僵尸进程，kill只能终止进程，而僵尸进程已经被终止了
+ 使用ps aux可以查看僵尸进程，名字会有[]括起来，而且进程状态是Z+
### 3、wait
+ pid_t wait(int * status);
+ 阻塞等待子进程退出（如果子进程没有结束，父进程会一直等）
+ 成功返回子进程pid，失败（无子进程）返回-1
+ 回收子进程的资源
+ 只能回收一个子进程，那个子进程先结束，就先回收哪个
+ 获取子进程结束状态（退出原因）使用wait的传出参数 int * status
+ WIFEXITD(status) != 0 进程正常退出，可使用下面的宏来获取进程的退出状态
	+ WEXITSTATUS（status）
+ WIFSIGNALED(status) != 0 进程异常终止，可使用下面的宏来来取得终止信号
	+ WTERMSIG(status)
+ WIFSTOPPED(status) != 0 进程处于暂停状态
	+ WSTOPSIG(status) 取得暂停信号
+ WIFCONTINUED（status）!= 0 进程暂停后继续运行  
### 4、waitpid
+ pid_t waitpid(pid_t pid, int * status, int options);
+ 成功返回回收子进程的pid 失败 返回 -1
+ 第一个参数 pid 
	+ pid > 0 指定回收子进程的pid
	+ -1 回收任意子进程（相当于 wait）
	+ 0 回收和当前调用 waitpid 一个组的所有子进程
	+ < -1 回收指定进程组内的任意子进程
		+ 使用ps ajx 可以查看 进程组id（PGID）+ 使用 -进程组id 这个参数可以回收指定组内的任意子进程
		+ 使用 kill -9 -进程组id 可以杀死整个组的进程
+ 第三个参数 可以指定父进程阻塞或不阻塞
	+ 如果设置为WNOHANG 不阻塞 ， 0 阻塞
	+ 如果设置为WNOHANG 
		+ 如果没有执行完的子进程 返回0
		+ 如果有执行完的子进程 返回该进程的pid
		+ 如果没有子进程 返回 -1
# 第四章： 管道
### 1、管道（匿名）
+ 一般指匿名管道
+ 管道只作用于有血缘关系的进程之间
+ 管道的性质
	+ 是一个伪文件，实质是内核缓存区
	+ 由两个文件描述符引用，一个表示读端，一个表示写端
	+ 规定数据从管道的写端流入，读端流出
	+ 管道的读写是阻塞的
+ 管道的原理，内核使用环形队列机制，借助内核缓冲区实现，默认大小4KB
+ 管道的局限性
	+ 一端不能读自己写的数据
	+ 数据一旦被读走，便不存在，不可反复读
	+ 半双工通信，一边读，另一边只能写
	+ 只能在有公共祖先（血缘关系）的进程间使用管道
### 2、创建匿名管道 pipe()
+ int pipe(int pipefd[2])
+ 成功返回 0 ，失败返回 -1 
+ 传出参数中 pipfd[0] : 读端， pipfd[1] : 写端
+ 每个进程拥有各自的pipefd[],里面是两个文件描述符，父进程在调用pipe()后，会在文件描述符表中保存pipefd[0] 和 pipefd[1]
+ fork()一定要在pipe()之后调用，才可以让父子进程通信，并且子进程也会自动保存pipefd[0] 和 pipefd[1]
+ 读端需要close(f[1])，写端需要close(f[0])
+ 如果父读子写，再想让父写子读，需要创建另一个管道
+ 兄弟之间通信，关闭父进程读和写的文件描述符
### 3、管道的读写行为
+ 管道读写的阻塞是因为文件的阻塞,管道是默认阻塞的伪文件
+ 读操作
	+ 有数据
		+ read()正常读，返回读出的字节数
	+ 无数据
		+ 写端全部关闭
			+ read()解除阻塞，返回0
			+ 相当于读文件读到尾部
		+ 写端没有全部关闭
			+ read()阻塞
+ 写操作
	+ 读端全部关闭
		+ 管道破裂，进程停止
			+ 内核给当前进程发信号SIGPIPE终止进程 
	+ 读端没有被全部关闭
		+ 缓冲区写满
			+ write()阻塞
		+ 缓冲区没满
			+ write()继续写
### 4、查看管道大小
+ ulimit -a 查看管道的缓冲区大小，也可以查看很多的虚拟地址空间的信息，比如栈大小，最大支持的文件描述符的个数
+ fpathconf 查看缓冲区大小，也可以查看很多文件的额信息，比如目录最长路径和文件名最大长度
### 5、设置管道读写非阻塞
+ 设置读端非阻塞 pipe(fd)
	+ fcntl() -变参函数，可以修改文件的属性flags（阻塞和非阻塞）
+ 方法
	+ 获取文件的flags
		+ int flags = fcntl(fd[0],F_GETFL);
	+ 将flags修改为非阻塞
		+ flags |= O_NONBLOCK
	+ 将修改后的flags设置回去
		+ fcntl(f[0],F_SETFL,flags);  
### 6、有名管道
+ 特点：
	+ 在磁盘上有一个文件，但是这个文件的大小一直是0，使用ls -l 可以查看 类型为p
	+ 在内核有一个缓存区
	+ 半双工
+ 使用场景
	+ 没有血缘关系的进程间通信
+ 创建方式
	+ 命令 mkfifo 管道名
	+ 函数 mkfifo(const char * pathname, mode_t mode)
+ fifo的使用方法和文件相同，使用open() close() read() write()（但是它和普通文件不同）

# 第五章：内存映射区
### 1、mmap()
+ mmap() 创建内存映射区（它不仅可以用于进程通信）
+ 作用：将磁盘文件的数据映射到内存，用户可以通过修改内存就能修改磁盘文件
	+ 映射内存的位置位于虚拟地址的动态库加载区
+ void * mmap
+ (
	+ void * adrr //映射区首地址，由内核指定，传NULL
	+ size_t length //映射区的大小 不能为0
		+ 实际大小是4kb的整数倍，如果指定的不是4k的整数倍，则向上取4kb的整数倍，例 4kb+1byte 映射成 8kb
	+ int prot //映射区操作权限 
		+ PROT_EXEC 执行
		+ PROT_READ 读 
		+ PROT_WRITE 写
		+ PROT_NONE 无
	+ int flages //标志位参数
		+ MAP_SHARED //共享 数据会同步到磁盘
		+ MAP_PRIVATE //私有 数据不会同步到磁盘
		+ MAP_ANON //创建匿名映射区
	+ int fd //映射的文件的文件描述符
		+ open() 获取
	+ off_t offset //映射文件的偏移量
		+ 可以只映射文件的一部分
		+ 必须是4kb的整数倍  
+ ) 
+ 返回值，调用成功，返回映射区的首地址，调用失败，返回MAP_FAILED（值为(void*)-1）
### 2、munmap()
+ 释放内存映射区（必须通过mmap返回的首地址，否则调用失败）
+ int munmap(void * addr, size_t length);
	+ addr 映射区的首地址
	+ length 映射区的长度
### 3、mmap注意事项
+ 在参数为 MAP_SHARED 的条件下，open的权限一定要 >= mmap的权限，否则mmap会调用失败（没有权限）
+ mmap的权限一定要有PROT_READ
+ open的时候O_CREAT一个新的文件，这个文件大小是0，没有空间，所以不能mmap这个文件，但是对这个文件做拓展，让这个文件大小不是0，就可以了
+ 再mmap之后，close掉文件描述符，对映射区的内存没有影响
+ 对ptr越界操作一般会出现段错误，如果这块内存没有被使用，就不会报错，但是是违法的
### 4、有血缘关系的进程使用mmap通信
+ 父子进程的内存映射区是共享的，使用ptr可以对同一块内存进行操作
### 5、mmap的特点
+ 内存映射区不会阻塞，需要考虑数据读取的先后顺序
### 6、创建匿名映射区
+ 用作有血缘关系的进程通信
+ 匿名映射区不需要打开文件（不需要映射文件）
+ mmap的参数
	+ length 设置一个合理的大小
	+ flags一定要|MAP_ANON来指定创建的是一个匿名映射区
	+ fd 传-1即可，表示没有文件描述符

### 7、无血缘关系的进程间通信
+ 不能使用匿名映射区
+ 借助一个磁盘文件创建映射
+ a.c:
	+ int fd = open("hello"，...);
	+ void * ptr = mmap(...fd,0);
	+ 对映射区做读写操作 使用ptr
+ b.c:
	+ int fd1 = open("hello",...);
	+ void * ptr = mmap(...fd1,0);
	+ 对映射区做读写操作 使用ptr
# 第六章：信号
### 1、信号的特征
+ 简单（内存开销小）
+ 不能携带大量的信息
+ 必须满足某个条件才能发送
### 2、信号的机制
+ A进程通过驱使内核给B进程发送一个信号，当B收到信号时，必须中断当前的进程，去处理信号，处理信号由B交给内核去处理
+ 信号是通过软件方式实现中断的，所以需要得到cpu的资源才能执行，所以有延时性，有不可靠性，系统负载越严重，信号的不可靠性越强
### 3、信号的产生
+ 按键产生，如：Ctrl + c  Ctrl + z Ctrl + \
+ 系统调用产生，如：kill, rase, abort
+ 软件条件产生， 如：定时器 alarm
+ 硬件异常产生，如：非法访问内存（段错误），除0（浮点数例外），内存对齐出错（总线错误）
+ 命令产生， 如：kill命令
### 4、信号的状态
+ 递达：递送并且到达进程
+ 未决：产生和递达之间的状态（未被处理），主要有阻塞（屏蔽）导致该状态
+ 阻塞信号集（信号屏蔽字）：阻塞状态的信号集合
+ 未决信号集：未决状态的信号集合
### 5、信号的处理方式
+ 执行默认动作
	+ Term 终止进程
	+ Ign 忽略信号，如:回收僵尸进程
	+ Core 终止进程，然后生成Core文件（检查进程死亡原因，主要用于gdb调试）
	+ Stop 暂停进程
	+ Cont 继续运行
+ 忽略（丢弃）
+ 捕捉（调用用户处理函数，而不是执行默认动作）
### 6、信号的4要素
+ 编号
+ 名称
+ 事件
+ 默认处理动作
### 7、kill -l 查看信号
+ 34 - 64 实时信号
+ man 7 signal 查看信号帮助文档
+ 9)SIGKILL 19)SIGSTOP 不允许忽略和捕捉，不允许设置为阻塞 
### 8、常见的信号
+ 终端按键产生
	+ Ctrl + c -> 2)SIGINT(终止/中断) "INT" --interrupt
	+ Ctrl + z -> 20)SIGTSTP(暂停/停止) "T" --Terminal 终端
	+ Ctrl + \ -> 3)SIGQUIT(退出)
+ 硬件异常产生信号
	+ 除0操作 ->8)SIGFPE(浮点数例外) "F" --float 浮点数
	+ 非法访问内存 ->11)SIGSEGV（段错误）
	+ 总线错误 ->7) SIGBUS
+ kill函数/命令产生信号
	+  int kill(pid_t pid, int sig);
	+  sig 信号
	+  pid > 0 发送信号指定的进程
	+  pid = 0 发送信号给与调用kill函数进程属于同一进程组的所有进程
	+  pid < 0 发送信号给 |pid| 进程组
	+  pid = -1 发送给进程所有权限发送的系统中的所有进程
+ raise函数 给当前进程发送指定的信号
	+ int raise（int sig）
+ abort函数，给当前进程发送异常终止信号 6)SIGABRT  
	+  void abort(void)
+ alarm设置定时器，每个进程只能有一个定时器
	+  unsigned int alarm (unsigned int seconds)返回0或剩余的秒数(上一次定时剩余的时间)，在指定seconds后，内核向当前进程发送终止信号 14)SIGALRM
	+ alarm(0) 取消定时器
	+ alarm()无论进程的状态如何，都会执行   
 + setitimer 设置定时器 精度可以达到us 可以周期定时
	 + int setitimer(int which, const struct itimerval * new_value struct itimerval *  old_value)
	 + which：指定定时方式
		 + 自然定时：ITIMER_REAL -> 14)SIGALRM 计算自然时间
		 + 虚拟空间计时（用户空间：ITIMER_VIRTUAL ->26)SIGVTALRM 只计算占用cpu的时间
		 + 运行时计时（用户+内核）：ITIMER_PROF ->27)SIGPROF 计算cup以及系统调用的时间
	 + it_interval 周期的时长
	 + it_value 定时的时长
	 
 
### 9、信号集操作
+ 不能直接对PCB中的阻塞信号集修改
+ 阻塞信号集影响未决信号集（未决信号集会在信号发送被阻塞后将对应的信号设置为1）
+ 加入阻塞信号集（信号屏蔽字）里的信号不能递达，除了9)SIGKILL 19)SIGSTOP
+ 自定义一个信号集 sigset_t set; //typedef unsigned long sigset t； 八个字节32位，每一位代表一个与之对应的信号编号，该位为0或1表示该信号存在或不存在
+ 自定义信号集的操作函数
	+ int sigemptyset（sigset_t * set） 将某个信号集全部置为0 
	+ int sigfillset(sigset_t * set) 将某个信号集全置为1
	+ int sigaddset(sigset_t * set, int signum) 将signum信号设置为1
	+ int sigdelset(sigset_t * set, int signum) 将signum信号设置为0
	+ int sigismember(const sigset_t *set, int signum) 判断某个信号是否在信号集中
	+ 关于信号操作函数 都是成功返回0 失败返回-1，对于sigismember()存在返回1，不存在返回0，出错返回-1
+ 使用自定义信号集来修改阻塞信号集
	+ int  sigprocmask(int how,const sigset\_t * set, siget_t * oldset) 成功：0 失败 -1
	+ 参数： 
		+ set 传入新的信号集地址
		+ oldset 传出旧的信号集地址
		+ how 的取值，假设阻塞信号集为mask
			+ SIG_BLOCK：set表示屏蔽信号集，即mask = mask | set
			+ SIG_UNBLOCK：set表示解除屏蔽信号集，即mask = mask & ~set
		+ SIG_SETMASK：set表示替换信号集，即mask = set 
+ sigpending函数
	+ 读取当前进程的未决信号集
	+ int sigpending(sigset_t * set) set传出参数， 成功：0 失败：-1 


### 10、信号捕捉
+ 简单捕捉 sighandler_t signal（int signum, sighandler_t handler）
	+ 将信号的默认动作改为新的函数（并不是signal函数捕捉的，而是内核）
	+ signum 捕捉的信号
	+ handler 函数指针,传递自定义的函数名 
	+ 返回值 为函数指针类型 typedef void(*sighandler_t)(int)
+ 复杂捕捉 int sigaction(int  signum, const struct sigaction * act, struct sigaction * oldact) 
	+ struct sigaction 
	+ {
	+ void (*sa_handler)(int) //信号执行函数
	+ void (*sa\_sigaction)(int, siginfo\_t *, void *)
	+ sigset\_t sa_mask //指定信号捕捉函数执行期间屏蔽的信号集
	+ int  sa_flags; // 0 使用默认属性（在信号函数执行期间，屏蔽新来的此信号）
	+ 
	+ }
+ sigaction注意事项
	+ 进程在运行信号捕捉函数时，PCB中的信号屏蔽字不生效，而是由sa_mask来屏蔽
	+ 进程在运行信号捕捉函数时，阻塞的常规信号不支持排队，产生的未决信号只有一个
### 11、信号捕捉的过程
+ 用户向内核递信号->内核捕捉信号->回到用户区调用用户的捕捉函数->返回给内核处理完成->回到用户区执行主进程

# 第七章：竞态条件（时序竟态）
### 1、pause()
+ int pause(void) 
+ 调用pause的进程会主动挂起，直到等到信号来唤醒
+ 等到的信号的处理动作是捕捉函数，那么pause会返回-1 并设置errno为EINTR
+ 如果收到的信号执行默认动作，如果是终止进程，则进程终止，如果是忽略，则进程继续处于挂起状态
### 2、时序问题
+ 以mysleep.c为例，由于用户进程可能会在某一时刻丢失cup，但是计时器依然计时，并且在进程未获得cpu时计时完成，发送信号，之后进程再次获得cpu，结果信号先被捕捉处理，再调用pause()，但是pause()再也无法等到那个SIGALRM信号了
### 3、解决时序问题
+ int sigsuspend(const sigset_t * mask) 和pause功能一样，但是会传一个mask，在这个函数执行期间，PCB的mask不生效，而是有这个参数里的mask生效，所以可以通过PCB的mask屏蔽信号，和sigsuspend里的mask解除信号的方式防止信号被提前处理
+ 时序问题无法通过调试工具查出，也不会报错，所以在写程序时一定注意避免出现时序问题
### 4、全局变量异步I/O
+ 以sync.c为例，如果在flag = 0;或flag = 1;的前面失去cpu，则会导致获得cpu后，进程会先处理信号，再给flag赋值，结果造成逻辑错误，进程阻塞
+ 当主进程和信号捕捉函数都对一个全局变量修改时，有可能会出现这种情况
+ 解决方法：
	+ 不要让主函数和信号捕捉函数都依赖于全局变量来控制逻辑
	+ 加锁
### 5、可重入/不可重入函数
+ 如果一个函数在执行到某一位置时再次调用该函数：如果会改变原函数里的值就是不可重入函数，不会改变原函数的值就是可重入函数
+ 避免出现不可重入函数，应该避免使用全局变量和malloc
+ 信号捕捉函数也要避免不可重入函数
+ 系统函数中的可重入函数查看 man 7 signal  
### 6、SIGCHLD信号
+ 产生条件：
	+  子进程终止
	+ 子进程接收到SIGSTOP信号暂停
	+ 子进程接收到SIGCONT后被唤醒
+ 接收SIGCHLD信号回收子进程
	+ 未决信号只有一个，但是子进程的PCB有多个，所以一个信号可能是多个子进程同时产生的 
### 7、信号传参
+ int sigqueue(pid_t pid, int sig, const union sigval value) 成功0，失败-1，向指定进程pid发送信号，第三个参数可以携带信息
	+ union sigval
	+ {
	+ int sival_int;
	+ void * sival_ptr;
	+ }  
+  sigaction捕捉sigqueue发送的信号时，结构体sigaction第二个成员来代替第一个，第4个成员的flags需要设置为SA_SIGINFO
	+  void (*sa_sigaction)(int, siginfo_t*,void *)
### 8、中断系统调用
+ 系统调用分为两类：
	+ 慢速系统调用：可能会使程序永久阻塞的一类，如read（读管道或终端），write（写管道或终端），wait，pause
	+ 其他系统调用：getpid，getppid，fork
+ 信号中断慢速系统调用：
	+ 信号不要被屏蔽
	+ 信号处理必须是捕捉（默认、忽略都不可以）
	+ 中断后返回-1，设置errno为EINTR（被信号中断） 
+ 可以修改sa_flags设置中断的系统调用重启或不重启，SA_INTERRUPT不重启,SA_RESTART重启 
	+ 修改sa_flags为SA_NODEFER，则在信号的捕捉函数执行期间，不阻塞该信号 

# 第八章：终端和守护进程
### 1、终端
+ 终端就是输入输出设备的总称
+ 字符终端：Alt + Ctrl + F1 到 F6
+ 图形终端：Alt + F7
+ 网络终端：SSH、Telnet
### 2、Linux系统的启动过程
+  init -> fork ->exec ->getty ->用户输入账号 ->login ->输入密码 -> exec ->bash
### 3、线路规程
+ 内核的线路规程会过滤特殊的字符比如Ctrl+c,这样的字符会被特殊处理，而不会调用read()写到终端上
### 4、进程组
+ 又叫作业
+ 父进程的pid就是组pid
+ pid_t getpgrp(void)获取当前进程组id
+ pid_t getpgid(pid_t pid)
+ int setpgid(pid_t pid, pid_t pgid)设置pid进程的进程组，如果没有会创建新的进程组
	+ 如果改变子进程的进程组，应在fork后 exec前调用
	+ 非root用户只能改变自己创建的子进程
### 5、会话
+ 会话可以包含多个组
+ setsid()创建会话
	+ 调用的进程不能是进程组组长，该进程会成为一个新进程组的组长和新会话的会话首进程
	+ 需要root权限（ubuntu不需要）
	+ 新会话会丢弃原有的控制终端，成为无终端进程
	+ 如果调用setsid()是组长进程，会出错返回
+ pid_t getsid(void) 获取当前会话id
### 6、守护进程
+ Daemon进程
+ 是Linux后台服务进程
+ 一般名字以d结尾
+ 没有终端，不受终端控制
+ 一般会永久运行直到系统关闭，有些会在完成指定任务后关闭
+ 不受用户登录/注销影响
+ 守护进程本质是个孤儿进程，所以必须要有父进程创建并将其父进程结束
+ 如：ftp服务器，nfs服务器
+ 创建守护进程的关键是调用setsid()创建新会话
+ 创建守护进程：
	+ 创建子进程，让父进程退出
	+ 在子进程中创建新的会话
		+ setsid()
		+ 创建新会话，让当前进程担任新会话的会话首进程，使其摆脱原有的终端控制
	+ 改变工作目录
		+ chdir()
		+ 防止占用可卸载的文件系统
	+ 重设文件权限掩码
		+ umask()
		+ 新创建出来的进程umask码继承自父进程，为了使它不受父进程umask掩码的影响，变为自己的风格
	+ 关闭文件描述符
		+ 守护进程没有终端，所以0~2的文件描述符没有用
		+ 一般会让0~2文件描述符指向/dev/null
		+ dup()/dup2()
	+ 执行守护进程的工作

# 第九章：线程
### 1、Linux的线程
+ 不同于windows的线程，Linux的线程本质是轻量级进程
	+ LWP：light wight process 
### 2、进程和线程的关系
+ 当一个进程创建一个线程时
	+ 它和创建的那个线程共享同一块虚拟地址空间，而不会创建新的虚拟地址空间，它们共同组成一个进程
	+ 每个线程都有各自的PCB
	+ 如果一个进程没有创建线程，那么这个进程就是一个线程
	+ 进程的主线程return 0，程序就会退出
### 3、 线程为什么快
+ 一个cpu只能执行一个操作，这种操作的最小单位就是线程
+ 对于一个任务，进程是只有一个操作，而多线程对应多个操作，所以多线程任务比单线程任务分配到更多的cpu资源
### 4、线程的实现原理
+ 线程和进程底层都会调用clone()
+ 线程的PCB不同，但指向相同的三级页表（所以多个线程共享一块物理地址空间）
+ ps -Lf 进程pid 
	+ 查看这个进程的线程
	+ LWP 线程号 是cpu分配时间轮片的依据（不是线程ID）
	+ NLWP 线程个数 
### 5、线程共享资源
+ 文件描述符表
+ 每种信号的处理方式
+ 当前工作目录
+ 用户ID和组ID
+ 共享虚拟地址空间(除了下面的非共享资源区)
### 6、线程独享资源
+ 线程ID
+ 处理器现场和栈指针(内核栈)
+ 栈
+ error变量
+ 信号屏蔽字
+ 调度优先级
### 7、线程的优\缺点（相比进程）
+ 优点：
	+ 提高并发性
	+ 内存开销小（在Linux里和进程相差不大）
	+ 数据通信、共享数据方便
+ 缺点：
	+ 调用的都是库函数，不稳定
	+ 调试、编写困难，gdb不支持
	+ 对信号支持不好
# 第十章：线程控制原语
+ 由于线程用的是第三方的库函数，编译连接时需要加-pthread
### 1、pthread_self() 获取当前进程ID
+ pthread_t pthread_self(void) 成功：0，失败：无
+ pthread_t 在Linux下类型为%lu，在其他系统中不一定是啥
+ 线程ID是进程区分自己的线程的标志，于其他进程无关
### 2、pthread_create() 创建线程
+ int pthread_create(pthread * thread, const pthread_attr_t * attr, void *(* start_routine)(void *), void * arg)
+ 参数：
	+ thread 传出 创建线程的ID
	+ attr 线程的属性，传NULL：使用默认属性  
	+ start_routine 线程执行的函数
	+ arg 线程执行函数的参数
+ 返回值 成功：0，失败：返回 error number
### 3、pthread_exit() 将当前线程退出
+ void pthread_exit(void * retval) 
	+ retval 线程退出状态，通常传NULL
+ 可以使用pthread_exit()代替主函数的return 0，这样不会退出进程，而是只退出主线程
+ 在线程主函数中调用return，会退出该线程
+ 在任意地方调用pthread_exit(),都会将该函数所属的线程退出
+ 在任意地方调用exit()，都会使进程退出
### 4、pthread_join() 回收线程
+ 任何线程都可以回收其他线程
+ int pthread_join(pthread_t pid, void ** retval);
+ pid：回收线程的ID
+ retval是一个二级指针局部变量，可以通过这个变量将（*reval）作为传出参数获取线程的退出状态（包括pthread_exit()的参数 和 线程返回的值）
+  如果线程被其他线程调用pthread_cancel()异常终止掉，(*reval)存放的是常数PHTREAD_CANCELED
+  返回值： 成功：0，失败：错误号，若无回收线程，调用失败，若有，阻塞等待该线程结束
### 5、pthread_detach() 分离线程
+ int pthread_detach(pthread_t thread)
	+ 分离ID 为 thread 的 线程，成功 ： 0，失败 ：错误号
+ 分离的线程自己会回收自己的PCB，其他线程无法回收（调用pthread_join会出错）
+ 也可以设置pthread_create() 参数2来设置线程分离
### 6、pthread_cancel()杀死线程
+ int pthread_cancel(pthread_t thread) 成功：0，失败：错误号
+ 调用后不会立刻杀死线程，而是等待一个取消点执行后杀死，取消点一般为：
	+ 大部分系统调用(或间接系统调用)，如 create，open，pause，close，read，write
	+ pthread_testcancel()设置取消点
+ 被杀死的线程，退出值是PTHREAD_CANCELED((void*)-1) 

# 第十一章：线程属性
### 1、线程属性结构体
+ 大多数情况使用默认属性
+ typedef struct
+ {
	+ int etachstate; //线程分离状态
	+ ……
	+ size_t guardsize; //线程栈末尾的警戒区大小，警戒区是线程栈之间的隔离
	+ ……
	+ size_t stacksize; //线程栈大小，默认均分进程栈
+ }pthread_attr_t
### 2、pthread_attr_init()线程属性的初始化
+ int pthread_attr_init(pthread_attr_t * attr) 成功：0，失败：错误号
+ 先初始化线程属性，再pthread_create创建线程
### 3、 销毁线程属性
+ int pthread_attr_destroy(pthread_attr_t * attr) 成功：0，失败：错误号
### 4、线程分离状态
+ 默认非分离状态
+ 必须使用已经初始化的线程属性
+ int pthread_attr_setdetachstate(pthread_attr_t * attr, int  detachstate)
	+ 设置分离状态
	+ 参数 detachstate
		+ PTHREAD_CREATE_DETACHED 分离状态
		+ PTHREAD_CREATE_JOINABLE 非分离状态，默认
+ int pthread_attr_getdetachstate(pthread_attr_t * attr, int * detachstate)
	+ 查看线程分离状态
	+ detachstate 传出分离状态
+ 如果通过线程属性设置分离，这个线程可能因为执行太快，在pthread_create调用没有返回之前就结束了，它结束时会将线程号和资源交给其他线程，之后pthread_create返回的是这个线程的线程号，而不是创建线程的线程号
	+ 这种情况几乎不会发生，除非线程很多
	+ 可以在线程函数中调用pthread_cond_timewait（），可以让线程睡一小会（足够pthread_create返回）
### 5、线程栈
+ int pthread_attr_setstack(pthread_attr_t * attr, void * stacaddr, size_t stacksize) 设置栈空间首地址和大小，用malloc()
+ int pthread_attr_getstack(pthread_attr_t * attr, void ** stacaddr, size_t stacksize) 获取栈空间首地址
+ int pthread_attr_setstacksize(pthread_attr__y * attr, size_t stacksize) 设置栈大小
+ int pthread_attr_getstacksize(pthread_attr__y * attr, size_t * stacksize) 获取栈大小
+ 可以通过设置栈大小来设置线程数量
### 6、线程注意事项
+ malloc和mmap可以被其他线程释放
+ fork()的进程只继承调用fork()线程
+ 在线程里尽量少用信号
+ NPTL
	+ 注意线程版本
	+ 命令:getconf GUN_LIBPTHREAD_VERSION

# 第十二章：线程同步
+ 当多个线程访问同一资源时，会产生与时间有关的错误，导致读写出错
### 1、锁
+ mutex 互斥量（互斥锁）
+ 锁不具有强制性（Linux建议锁），线程可以选择不加锁
+ 线程访问数据时，必须解锁才能访问
### 2、pthread_mutex_t 类型 
+ 本质是一个结构体
+ 该类型的变量只有 1 0 两种值 
### 3、pthread_mutex_init() 初始化锁 
+ int pthread_mutex_init(phtread_mutex_t * restrict mutex, const pthread_mutexattr_t * restrict attr)
	+ mutex 锁指针
	+ attr 锁得属性
	+ restrict 被这个词修饰的指针，只能用该指针修改内存
+ 调用之后的mutex值为1
### 4、pthread_mutex_destroy() 销毁锁
+ int pthread_mutex_destroy(pthread_mutext_t * mutex)
### 5、pthread_mutex_lock() 加锁
+ int pthread_mutex_lock(pthread_mutex_t * mutex)
+ 实质是mutex--
+ 如果锁住，则会阻塞直到被解锁
### 6、pthread_mutex_trylock(pthread_mutex_t * mutex)
+ 作用同pthread_mutex_lock()，但是不阻塞
+ 如果锁住，则直接返回
### 7、pthread_mutex_unlock() 解锁
+ int pthread_mutex_unlock(pthread_mutex_t * mutex)
+ 实质是mutex++
### 6、所得 粒度 越小越好
+ 访问完资源后立刻解锁
### 7、死锁
+ 线程对一个互斥量加锁2次
	+ 解决：在加锁前一定要判断/解锁
+ 线程A拥有A锁，请求B锁，线程B拥有B锁，请求A锁
	+ 使用tylock，当A获取不到B时，主动放弃A锁
+ 震荡：对于第二种死锁，A、B同时放弃自己的锁，再同时拿起，依然是死锁的，见哲学家用餐 
### 8、读写锁
+ 一把锁有三种状态
	+ 读模式下加锁（读锁）
	+ 写模式下加锁（写锁）
	+ 不加锁 
+ 写独占：加写锁时，其他线程不能加锁
+ 读共享：加读锁时，其他线程可以加读锁（如果这几个线程里有想加写锁的，则其他线程不能加读锁，因为写锁优先级高）
+ 写锁优先级高：多个线程同时加锁，先加写锁
### 9、读写锁函数
+ pthread_rwlock_t 读写锁类型
+ pthread_rwlock_init
	+ 初始化，参数：读写锁地址，属性
+ pthread_rwlock_destroy
+ pthread_rwlock_rdlcok
+ pthread_rwlock_wrlock
+ pthread_rwlock_tryrdlock
+ 不阻塞
+ pthread_rwlock_trywrlock
+ pthread_rwlock_unlock
+ 以上函数 成功：0，失败：错误号
+ 当读操作相对很多时，读写锁并行效率要比互斥锁高
### 10、条件变量
+ 本身不是锁，但可以造成线程阻塞，通常与互斥锁配合，给多线程一个会合的场所
+ pthread_cond_t
+ pthread_cond_init
+ pthread_cond_destory
+ pthread_cond_wait
	+ int pthread_cond_wait(pthread_cond_t * restrict cond, pthread_mutex_t * restrict mutex)
	+ 两个参数必须是初始化好的
	+ 阻塞等待cond被唤醒
	+ 解锁互斥锁， 相当于pthread_unlock()
		+ 1、2两步为一个原子操作
	+ 当被唤醒时，pthread_cond_wait()函数返回时，解除阻塞，并申请加锁互斥锁，相当于pthread_mutex_lock()
+ pthread_cond_timedwait
	+ 第三个参数 const struct timespec * restrict abstime 
		+ 阻塞等待这一段时间后返回
		+ 是一个绝对时间类型
		+ 相对于 1970/1/1/00:00:00
		+ 正确用法
			+ time_t cur = time(NULL);
			+ struct timespec t; 
			+ t.tv_sec = cur+1; //定时1s
				+ t.tv_nsec = cur + 1 //定时1ns
+ pthread_cond_signal
	+ 当条件变量cond满足时，另外一个线程会通过该函数唤醒阻塞在该条件变量上的一个线程 
+ pthread_cond_broadcast
	+ 和pthread_cond_signal相似，但是唤醒的是阻塞在该条件变量上的所有线程 
+ 生产者消费模型，见cond.c 
### 12、信号量
+ 进化版的互斥锁 可以从 0~n 取值
+ 信号量的初值决定占用信号量的最大线程数量
+ 也可用于进程
+ sem_t
+ sem_init
	+ int sem_init(sem_t * sem, int pshared, unsigned int value)
	+ pshared 是否可以在进程间共享
	+ value 信号量的取值 
+ sem_destroy
+ sem_wait
	+ 加锁，sem--
	+ 只有当信号量==0时，阻塞
+ sem_trywait
+ sem_timedwait
+ sem_post
	+ 解锁，sem++
+ 成功返回0 失败返回-1
+ 生产者消费者模型，见sem.c
### 13、进程通信mutex
+ 指定pthread_init的第二个参数类型
+ pthread_mutexattr_t mattr 锁属性
+ pthread_mutexattr_init()
+ pthread_mutexattr_destroy()
+ pthread_mutexattr_setpshared() 参数2取值
	+ PTHREAD_PROCESS_PRIVATE(线程锁，默认)
	+ PTHREAD_PROCESS_SHARED(进程锁)
### 14、文件锁
+ struct flock lock;
+ //加锁
+ lock.l.type = F_RDLOCK; //读锁
+ fcntl(fd. F_SERLKW, &lock);
+ //解锁
+ lock.l.type = F_UNLOCK;
+ fcntl(fd, F_SETLKW, &lock);
+ 写锁独占读锁共享
+ 多线程之间不能使用文件锁，因为文件描述符文件结构体共享
### 15、哲学家用餐
+ 震荡：死锁的第三种方式
+ 让一个线程拿相反的筷子，这样就会和另一个线程抢一个筷子。这样一定会有一个线程拿到两把筷子
