import time

def fib(x)
    if (x <= 2)
        return 1
    end
    return fib(x - 1) + fib(x - 2)
end

c = time.clock()
i=0
while(i <= 40)
    print("fib:", fib(i)," n: ",i) # minimum stack size: 78!!
    i=i+1
end
print("time:", time.clock() - c, 's')


