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
    "torque",
    1,
    mpr.Type.FLOAT, 'angle', -5, 5, None, None
)
sig_mode = dev.add_signal(
    mpr.Direction.OUTGOING,
    "mode_1",
    1,
    mpr.Type.INT32, 'angle', 0, 1, None, None
)

temp = 0.0


while True:
    dev.poll(100)
    sig_in.set_value(2)
    sig_mode.set_value(1)