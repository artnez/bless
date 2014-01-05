BLESS
=====

**B**luetooth **L**ow **E**nergy **S**canning **S**ystem. A device that
maintains an ephemeral log of nearby peripherals broadcasting on the
[Bluetooth low energy][ble] (BLE) protocol.

Requirements
------------

Tested on [Arduino UNO R3][arduino].

Requires a [BLE Mini][blemini] module. The firmware more or less communicates
directly with the [CC2540][cc2540] on the BLE Mini. It shouldn't be hard to
adapt to any board using CC2540 so long as the I/O abstractions are similar.

The firmware implements the [Bluetooth HCI][blehci] controller stack to
interface with the BLE hardware.

Deployment
----------

Use [ino ~0.3.6][ino] to compile and upload. Default command options are stored
in `ino.ini`. Log data is sent to `Serial 0` while the device is running.


[arduino]: http://arduino.cc/en/Main/arduinoBoardUno 
[ble]: http://wikipedia.org/wiki/bluetooth_low_energy
[blehci]: https://docs.google.com/file/d/0B1u5C76zhFjTRGZ2WlZ4a3o5cms
[blemini]: http://redbearlab.com/blemini/
[cc2540]: http://ti.com/product/cc2540
[ino]: http://inotool.org/
