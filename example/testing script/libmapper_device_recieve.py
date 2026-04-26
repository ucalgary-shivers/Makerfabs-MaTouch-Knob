import libmapper as mpr


def on_input(signal, event, id, value, time):
    # try:
    #     if event == mpr.Signal.Event.REMOTE_UPDATE:
    #         print(f"Received value: {value}")
    #     else:
    #         print('Else')
    # except Exception as e:
    #     print('Exception',e)
    print(signal,event,value)


dev = mpr.Device("my_device")

sig_in = dev.add_signal(
    mpr.Direction.INCOMING,
    "in",
    1,
    mpr.Type.FLOAT, 'angle', 0, 3600, None, on_input
)




while True:
    dev.poll(100)