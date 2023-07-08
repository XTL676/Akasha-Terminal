# Akasha Terminal

## 虚空终端系统

------

### 系统类

#### 抽象类（Abstract）

##### AbstractSystem

系统类的顶级父类，抽象类，用以派生各种各样的系统类

##### AbstractKernelSystem

内核里系统类的父类，抽象类，继承与**AbstractSystem**

##### AbstractTerminalSystem

终端界面里的系统类的父类，抽象类，继承与**AbstractSystem**

#### 子类

------

### 管理类

#### TerminalManager

终端管理器类，专门管理终端Terminal下的所有类。

#### KernelManager

内核管理器类，专门管理内核Kernel下的所有类。

------

### 权限与特权(Permission & Privilege)

在Akasha中，

**权限**表示某个用户所能执行的操作；

**特权**表示系统或更高级别的用户授予低级别用户能进行的操作（权限），主要讲究的是授予权限的过程。
*例如：用户A有创建文件的权限，管理员A授予用户A删除文件的特权，则用户A现在有创建文件、删除文件的权限。*

#### 无权限访问(No Permission Access)


指当前用户没有执行此操作的权限。

#### 用户无特权(Invalid Privilege)


指当前用户没有授予其它用户某个权限的特权。

------

### 系统目录

```cmd
/---- 根目录
	|--- boot	存放的是启动 Akasha 时使用的一些核心文件，包括一些连接文件以及镜像文件
	|     |--- grub 存放系统启动配置文件
	|	  |--- kakasha 存放内核文件
	|--- conf	存放所有的系统管理所需要的配置文件和子目录
	|--- root	该目录为系统管理员，也称作超级权限者的用户主目录
	|--- sapp   存放的是系统管理员使用的系统管理程序	
	|--- tmp	存放一些临时文件
	|--- user	存放用户文件夹
	|	  |--- usr1 --- home 用户名命名的文件夹 --- 用户主目录
	|	  |--- usr2 --- home
	|	  |--- ... --- ...
	|--- app	存放应用程序
```

