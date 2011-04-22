'''
Created on Apr 8, 2011

@author: erublee
'''
import ecto,buster


def test_reconnect():
    plasm = ecto.Plasm()
    g = buster.Generate(start=2, step=2)    
    m = buster.Multiply(factor=2)
    m2 = buster.Multiply(factor=2)
    gather = buster.Gather_double(n=2)
    plasm.connect(g, "out", m , "in")
    plasm.connect(g, "out", m2 , "in")
    try:
        plasm.connect(m2,"out",m,"in")
        assert(False)
    except RuntimeError,e:
        print "Reconnect caught: ",e
    plasm.connect(m2, "out", gather , "in_0000")
    plasm.connect(m, "out", gather , "in_0001")
    try:
        plasm.connect(m2, "out", gather , "in_0001")
        assert(False)
    except RuntimeError,e:
        print "Reconnect caught: ",e
    
    #check some values
    plasm.go(gather)
    print gather.outputs.out
    assert(gather.outputs["out"].val == 2 *(2*2))
    plasm.go(gather)
    #should remain unchanged
    assert(gather.outputs["out"].val == 2*(2*2))
    plasm.mark_dirty(g) #mark top of tree dirty (propagates down)
    plasm.go(gather)
    assert(g.outputs["out"].val == 4)
    assert(gather.outputs["out"].val == 2*(2*4)) #g should be at 4 here
    #ecto.view_plasm(plasm)

if __name__ == "__main__":
    test_reconnect()