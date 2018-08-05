def judge(t, b): #s 是字符串，b 是变量的list
    print('judge: ' + t)
    while t[0] == ' ':
        t = t[1:]
    while '  ' in t:
        t = t.replace('  ',' ')
    for i in range(1, len(t)-2):
        if t[i] == ' ' and t[i-1] != '-' and t[i-1] != ','and t[i-1] != '+' and t[i-1] != '*' and t[i-1] != '/' and t[i-1] != '(' and t[i-1] != ')' and t[i+1] != '-' and t[i+1] != ',' and t[i+1] != '+' and t[i+1] != '*' and t[i+1] != '/' and t[i+1] != '(' and t[i+1] != ')':
            return False
    s = t.replace(' ', '')
    if jgnum(s) == False:
        return False
    s = s + ')'
    for i in range(0, len(s) - 2):
        if s[i] >= '0' and s[i] <= '9':
            s = s[:i]+'0'+s[i+1:]
            if (s[i + 1] >= '0' and s[i + 1] <= '9') or s[i+1] == '.':
                s = s[:i + 1] + '@' + s[i + 2:]
        elif s[i] == '@':
            if (s[i + 1] >= '0' and s[i + 1] <= '9') or s[i+1] == '.':
                s = s[:i + 1] + '@' + s[i + 2:]
    s = s[:len(s) - 1]
    s = s.replace('@', '')
    b = b+['0']
    for i in range(0,len(s)-2):
        if s[i] == '+' and s[i+1] == '-':
            s = s[:i]+'@'+s[i+1:]
    s = s.replace('@', '')
    if '(' not in s:
        return jgbasic(s, b)
    else:
        i = 0
        cur = 0
        j = len(s)-1
        while j >= 0:
            if s[j] == ')':
                i += 1
                cur = j
            elif s[j] == '(':
                i -= 1
                start = push(s, j)
                end = cur+1
                if basic(s[start:end], b):
                    alt = b[0]
                    news = alt.join([s[0:start], s[end:len(s)]])
                    return judge(news, b)
                else:
                    return False
            j -= 1
            if i < 0:
                return False
        if i != 0:
            return False
        else:
            return True


def push(s, start):# s 是字符串， start是左括号出现的位置，返回运算符出现的位置
    if start == 0:
        return 0
    else:
        while start != 1:
            if judgefunc(s, start-1):
                return start
            start -= 1
    if s[0] == '(' or s[0] == '-' or s[0] == '+':
        return 1
    else:
        return 0


def judgefunc(s, cur): # 判断是不是运算符，help push
    if s[cur] == '+' or s[cur] == '-' or s[cur] == '/' or s[cur] == '*' or s[cur] == '(':
        return True
    else:
        return False


def basic(s, b): # 判断最基本运算单元
    if s[0] == '(':
        return jgbasic(s[1:len(s)-1], b)
    index = s.index('(')
    operator = s[0:index]
    value = s[index+1:len(s)-1]
    if operator == 'pow':
        return jgpow(value, b)
    elif operator in ['pow', 'sin', 'cos', 'tan', 'log', 'abs', 'sqrt', 'asin', 'atan', 'acos']: #可加
        return jgother(value, b)
    else:
        return False


def jgpow(value, b): # 判断pow 函数
    if ',' not in value:
        return False
    else:
        index = value.index(',')
        return jgbasic(value[0:index], b)*jgbasic(value[index+1:len(value)], b)


def jgother(value, b): # 判断其他函数，就是判断四则运算
    return jgbasic(value, b)


def jgbasic(s, b): # 判断最基本的四则运算
    if s[0] == '-':
        s = s[1:len(s)]
    marks = ['+', '-', '*', '/']
    cur = 0
    for i in range(0, len(s)):
        if s[i] in marks:
            if s[cur:i] not in b:
                return False
            cur = i+1
    if s[cur:len(s)] in b:
        return True
    else:
        return False


def jgnum(s):
    s = s+')'
    for i in range(0,len(s)-2):
        if s[i] >= '0' and s[i] <= '9' and s[i+1] >= '0' and s[i+1] <= '9':
            s = s[:i+1]+'@'+s[i+2:]
        elif s[i] == '@' and s[i+1] >= '0' and s[i+1] <= '9':
            s = s[:i+1]+'@'+s[i+2:]
    s = s[:len(s)-1]
    t = s.replace('@', '')
    if len(t) <= 3:
        return True
    for i in range(0,len(t)-3):
        if t[i] == '.' and t[i+1] >= '0' and t[i+1] <= '9' and t[i+2] == '.':
            return False
        if t[i] == '.' and t[i+1] == '.':
            return False
    if t[len(t)-3] == '.' and t[len(t)-2] >= '0' and t[len(t)-2]<= '9' and t[len(t)-1] >= '0' and t[len(t)-1] <= '9':
        return False
    return True


def isallnum(s):
    j =0
    for i in s:
        if i >= '0' and i <= '9':
            j += 1
    return j == len(s)



