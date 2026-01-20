import libmapper as mpr


def on_input(signal, event, id, value, time):
    try:
        if event == mpr.Signal.Event.UPDATE:
            print(f"Recieved value: {value}")
        else:
            print('Else')
    except:
        print('Exception')
    

dev = mpr.Device("my_device2")

sig_in = dev.add_signal(
    mpr.Direction.OUTGOING,
    "out",
    1,
    mpr.Type.FLOAT, 'angle', 0, 3600, None, None
)


temp = 0.0


while True:
    dev.poll(100)
    sig_in.set_value(temp)
    print("send",temp)
    temp = temp + 0.01