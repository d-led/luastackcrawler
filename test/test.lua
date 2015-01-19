function assert() print 'pending a correct test' end

assert(trystack(1)=='1 ')
assert(trystack(nil)=='nil ')
assert(trystack("1")=='1 ')
assert(trystack("1","2")=='1 2 ')
assert(trystack({1,2})=='{1->1 2->2} ')
assert(trystack(true,false,3)=='true false 3 ')

t={}
t[1]="1"
t[2]="2"
local expected = '1 {1->1 2->2} '
assert(trystack("1",t)==expected)

t["1"]=1
t["2"]={3,4,5}
expected = 'nil {1->1 2->2 1->1 2->{1->3 2->4 3->5}} '
assert(trystack(nil,t) == expected)

a={}
a[a]="x";
a[t]=t;
a[{1,2}]={3}

expected=[[{table*->{1->1 2->2 1->1 2->{1->3 2->4 3->5}} table*->x {1->1 2->2}->{1->3}} ]]
io.write('got     : ')
local res=trystack(a)
print('expected: '..expected)
assert(res==expected)

io.write('got     : ')
res=trystack2(1,2)
expected='1 2 '
print('expected: '..expected)
assert(res==expected)

io.write('got     : ')
res=tryextension("begin",a)
expected=[[begin {table*->{1->1 2->2 1->1 2->{1->3 2->4 3->5}} table*->x {1->1 2->2}->{1->3}} ]]
print('expected: '..expected)
assert(res==expected)

io.write('got     : ')
expected=expected.."end"
res=trystackextension("begin",a,"end") --should produce the same result
print('expected: '..expected)
assert(res==expected)

tryluaref(tryluaref)
tryluaref(LuaRef)
tryluaref("bla")
tryluaref(false)
tryluaref({})

assert( ArraySize { 1, 2, 5, bla='7' } == 3 )
