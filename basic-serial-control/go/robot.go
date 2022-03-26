package basic_serial_control

import (
	"errors"
	"unsafe"

	"go.bug.st/serial"
)

type Robot interface {
	Connect() error
	Disconnect() error
	Move(throttle int8, steer int8) error
}

type robot struct {
	port serial.Port
}

func NewRobot() Robot {
	return &robot{}
}

func (r *robot) Connect() error {
	mode := &serial.Mode{
		BaudRate: 9600,
	}

	port, err := serial.Open("/dev/ttyACM0", mode)
	if err != nil {
		return errors.New("could not open serial port")
	}

	r.port = port
	return nil
}

func (r *robot) Disconnect() error {
	if r.port == nil {
		return errors.New("robot not connected")
	}

	r.port.Close()
	r.port = nil
	return nil
}

func (r *robot) Move(throttle int8, steer int8) error {
	if r.port == nil {
		return errors.New("robot not connected")
	}

	_, err := r.port.Write([]byte{intToByte(throttle), intToByte(steer)})

	return err
}

func intToByte(v int8) byte {
	return *(*byte)(unsafe.Pointer(&v))
}
