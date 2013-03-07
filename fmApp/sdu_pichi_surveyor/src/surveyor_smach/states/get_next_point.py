import rospy
import smach
import smach_ros

class getNextPosition(smach.State):
    """
        Temporary implementation. State giving the next position goal
        TODO: Must be implemented as a service interfacing to a file
    """
    def __init__(self):
        smach.State.__init__(self, outcomes=['succeeded'], output_keys=['next_x','next_y'])
        self.ptr = 0
        self.position_list =[[2,2],[-3,-3],[1,-1],[-2,2],[2,-4]]

    def execute(self, userdata):
        userdata.next_x = self.position_list[self.ptr][0]
        userdata.next_y = self.position_list[self.ptr][1]
        self.ptr = self.ptr + 1
        if self.ptr == len(self.position_list) - 1 :
            self.ptr = 0
        return 'succeeded'       