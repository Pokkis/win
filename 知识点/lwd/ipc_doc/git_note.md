#### git 参考
```
Git 命令大全
https://www.jianshu.com/p/93318220cdce

Git 工作流程
https://www.ruanyifeng.com/blog/2015/12/git-workflow.html
https://www.cnblogs.com/cnblogsfans/p/5075073.html
https://www.jianshu.com/p/34b95c5eedb6

如何看待 Git flow 发明人称其不适用于持续交付？
https://www.zhihu.com/question/379545619
```

#### git 常用命令
```
克隆远端仓库到本地
git clone url_path

添加修改到暂存区
git add xxx

提交暂存区的修改到本地版本管理
git commit -m "modify log info"

推送本地修改到远端服务器
git push

拉取远端服务的修改
git pull

查看远程仓库地址
git remote -v

查看历史提交记录
git log

回退到上一版本
git reset --hard HEAD^

回退到某一版本
git reset --hard id前几位
```

#### git 误提交退回
```
git add 操作时，有时会误添加一些不想提交的文件，如何解决？
1、误add单个文件
git reset HEAD 将file退回到unstage区
2、误add多个文件，只撤销部分文件
git reset HEAD 将file退回到unstage区
```

#### git rm 与 git reset 区别
```
git rm 与 git reset的区别
git rm：用于从工作区和索引中删除文件
git reset：用于将当前HEAD复位到指定状态。一般用于撤消之前的一些操作(如：git add,git commit等)。

git rm file_path 删除暂存区和分支上的文件，同时工作区也不需要
git rm --cached file_path 删除暂存区或分支上的文件, 但工作区需要使用, 只是不希望被版本控制（适用于已经被git add,但是又想撤销的情况）
git reset HEAD 回退暂存区里的文件
```

#### git diff 比较差异
```
git diff 比较的是工作区和暂存区的差别
git diff --cached 比较的是暂存区和版本库的差别
git diff HEAD 可以查看工作区和版本库的差别
```

#### git 放弃修改
```
场景1.本地修改尚未提交到暂存区， 即本地代码修改还没有执行(git add)，使用命令
git checkout -- fileName
// 放弃全部修改：
git checkout .

场景2. 本地修改已经先提交暂存区，执行的git add,但没有提交git commit， 撤销缓存提交
// 放弃修改指定文件
  git reset HEAD filepathname 

// 放弃修改所有文件
git reset HEAD .

Note: 只是撤销缓存提交，本地代码修改尚未还原。

场景3. 已经git commit 提交，版本回退
// 回退到前一个版本
git reset --hard HEAD^

//回退到指定版本
 git reset --hard  commitid  

Note:
HEAD表示当前版本， HEAD^表示上个版本， HEAD^^表示上上个版本
commitid通过git log获取，直接使用版本号的前几个字符串即可。(git log --pretty=oneline 一行显示)
要重返未来，用git reflog查看命令历史，以便确定要回到未来的版本
```

#### git 查看和修改用户、邮箱

```
git 查看用户名:git config user.name
git 切换用户:git config --global user.name "xxx"
git 切换邮箱:git config --global user.email "xxx"。 
```

#### Windows10如何更换Git用户名和密码

```
控制面板 --》 用户账户 --》 Windows 凭据 --》普通凭据某项编辑或删除
```

#### git diff

```
git diff ： 比较工作区和暂存区之间的差异（git add）
git diff HEAD [ <path> … ] ：比较工作区与最新本地仓库之间的差异
git diff --cached [ <path>… ] ：比较暂存区与最新本地仓库（本地仓库最近一次commit的内容）的差异

git diff commit-id [ <path>… ] ：比较工作区与指定commit-id的差异
git diff --cached [ <commit-id> ] [ <path>… ] ：比较暂存区与指定commit-id的差异
git diff [ <commit-id> ] [ <commit-id> ] ：比较两个commit-id之间的差异

Git Diff中文乱码问题
在命令行查看git的diff，碰到GBK编码的文件时，中文显示会是乱码。
原因：git diff的输出使用less，而less不支持gbk编码
解决方案：通过将git diff的输出放到一个支持gbk的编辑器中就可以了

以下是使用不通的编辑器查看git diff的办法。
Vim
git diff | vim -R -

MacVim
git diff | mvim -R -

Sublime
git diff | subl

TextMate
git diff | mate

vim中使用gbk编码
用vim打开文件后，如果vim无法正确识别文件的编码，通过下面命令可以通知vim使用指定编码重新加载文件：
:e ++enc=<encoding>

fileencoding配置项
vim的这个配置项的值是一个编码列表。
当vim打开一个文件时，会按照顺序尝试使用这个列表中的编码来显示文件，并使用第一个能够正确显示文件的编码来打开文件，更多细节内容参考vim文档。
在.vim/vimrc中设置fileencodings，自动识别gbk编码
set fileencodings=ucs-bom,utf-8,utf-16,gbk,latin1
```

#### 解决git status不能显示中文
```
原因
在默认设置下，中文文件名在工作区状态输出，中文名不能正确显示，而是显示为八进制的字符编码。

解决办法
将git 配置文件 core.quotepath项设置为false。
quotepath表示引用路径
加上--global表示全局配置
git bash 终端输入命令：
git config --global core.quotepath false

注：遇到设置了以上，git bash 中输入 git status 时还是乱码。但是再 win 下的 Powershell 、vscode 和 linux 下正常。卸载后安装新的版本后正常 git verseion 2.29.2.windows.1。
```

#### git bash 终端乱码
```
设置git 的界面编码：
git config --global gui.encoding utf-8
设置 commit log 提交时使用 utf-8 编码：
git config --global i18n.commitencoding utf-8
使得在 $ git log 时将 utf-8 编码转换成 gbk 编码：
git config --global i18n.logoutputencoding gbk
使得 git log 可以正常显示中文：
export LESSCHARSET=utf-8
经过以上步骤，我们会发现可以正常显示中文注释了。
```
注：
```
git config --global core.quotepath false # 设置 git status utf-8编码
git config --global gui.encoding utf-8 # 设置Git GUI界面utf-8编码
git config --global i18n.commit.encoding utf-8 #设置commit信息utf-8编码
git config --global i18n.logoutputencoding utf-8 # 设置输出 log utf-8 编码
```

#### git提交忽略文件或文件夹
```
在项目根目录下面 添加 .gitignore文件
文件中每一行表示需要忽略的文件的正则表达式。

.gitignore文件过滤有两种模式，开放模式和保守模式

- 开放模式负责设置过滤哪些文件和文件夹

    例如：
/target/ 表示项目根目录下的target文件夹里面所有的内容都会被过滤，不被 GIT 跟踪
.classpath 表示项目根目录下的.classpath文件会被过滤，不被GIT跟踪

- 保守模式负责设置哪些文件不被过滤，也就是哪些文件要被跟踪。

    例如：
!/target/*.h 表示target文件夹目录下所有的.h文件将被跟踪

如果你不清楚GIT会跟踪哪些文件，你可以先用git status命令查看一下。另外别忘记了在.gitignore文件中加入一行.gitignore，否则的话.gitignore将会被传到GIT服务器上。

规则写法
1. 在已忽略文件夹中不忽略指定文件夹
/node_modules/*
!/node_modules/layer/

2. 在已忽略文件夹中不忽略指定文件
/node_modules/*
!/node_modules/layer/layer.js

注：要忽略的文件夹一定要结尾 /* ，否则不忽略规则将无法生效

3. 其他规则写法
以斜杠“/”开头表示目录；
以星号“*”通配多个字符；
以问号“?”通配单个字符
以方括号“[]”包含单个字符的匹配列表；
以叹号“!”表示不忽略(跟踪)匹配到的文件或目录；
3.1. 忽略文件
    *.bak               # 忽略所有扩展名为.bak的文件
    !keep.bak      # 但keep.bak文件除外（不会被忽略）
    temp/test.txt  # 忽略temp目录下的test.txt文件
    temp/*.txt       # 忽略temp目录下所有扩展名为.txt的文件
 
3.2. 忽略目录
    temp/       # 忽略temp目录下的所有目录和文件
    temp/*/    # 忽略temp目录下的所有目录，但不会忽略该目录下的文件
```
