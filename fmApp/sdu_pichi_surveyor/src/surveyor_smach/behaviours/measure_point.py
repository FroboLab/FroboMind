import rospy
import smach
import smach_ros
import actionlib
from surveyor_smach.states import get_next_point
from generic_smach.states import wait_state
from position_action_server import *
from position_action_server.msg import *

def build():
    behaviour = smach.StateMachine(outcomes=['success','preempted','aborted'])
    with behaviour :
        smach.StateMachine.add('GET_NEXT', get_next_point.getNextPosition(), 
                               transitions={'succeeded':'GO_TO_POINT'})                    
        smach.StateMachine.add('GO_TO_POINT', 
                               smach_ros.SimpleActionState('/positionActionServer',positionAction, goal_slots=['x','y']),
                               transitions={'succeeded':'MEASURE','preempted':'preempted','aborted':'aborted'},
                               remapping={'x':'next_x','y':'next_y'})
        smach.StateMachine.add('MEASURE' , wait_state.WaitState(3),
                               transitions={'succeeded':'GET_NEXT','preempted':'preempted'})
        
    return behaviour