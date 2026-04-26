import libmapper as mpr
import time

print(mpr.__version__)
graph = mpr.Graph()
#graph.set_interface("wifi")
graph.poll(100)


map = graph.maps()

print(len(map))


device = graph.devices()
for i in device:
    print(i)
    print(mpr.Object.get_property(i,"name"))

signals = graph.signals()
#dev_esp = mpr.Map(dst_sig, src_sig1)
print(len(signals))

mylist = {}
for i in signals:
    print(i)
    print(i.id)
    print(mpr.Object.get_property(i,"name"))
    mylist[mpr.Object.get_property(i,"name")] = i
dev_esp = mpr.Map( mylist["out"], mylist["in"])
print(dev_esp.get_status())
dev_esp.push()
print(dev_esp.get_status())

while True:
    time.sleep(1)
    print(dev_esp.get_status())
    
    print(dev_esp.get_is_ready())
    
    graph.poll(100)


