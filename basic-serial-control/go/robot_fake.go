package basic_serial_control

import "fmt"

type fakeRobot struct{}

func NewFakeRobot() Robot {
	return &fakeRobot{}
}

func (*fakeRobot) Connect() error {
	fmt.Println("fake robot conected")
	return nil
}
func (*fakeRobot) Disconnect() error {
	fmt.Println("fake robot disconnected")
	return nil
}
func (*fakeRobot) Move(throttle int8, steer int8) error {
	fmt.Printf("fake robot: move throttle=%d steer=%d\n", throttle, steer)
	return nil
}
