#/****************************************************************************
# FroboMind wii_states.py
# Copyright (c) 2011-2013, author Leon Bonde Larsen <leon@bondelarsen.dk>
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#    * Redistributions of source code must retain the above copyright
#      notice, this list of conditions and the following disclaimer.
#    * Redistributions in binary form must reproduce the above copyright
#      notice, this list of conditions and the following disclaimer in the
#      documentation and/or other materials provided with the distribution.
#    * Neither the name FroboMind nor the
#      names of its contributors may be used to endorse or promote products
#      derived from this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
# ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
# WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
# DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
# (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
# LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
# ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
# SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#****************************************************************************/
import rospy
import smach
import smach_ros

class remoteControlState(smach.State):
    """
        Implements remote controlled operation as a smach state
        Usage example:
            smach.StateMachine.add('REMOTE_CONTROL', remoteControlState(self.hmi), transitions={'enterAutomode':'AUTO_MODE'})
    """
    def __init__(self,hmi):
        smach.State.__init__(self, outcomes=['enterAutomode','preempted'])
        # Instantiate HMI
        self.hmi = hmi

    def execute(self, userdata):
        self.r = rospy.Rate(self.hmi.publish_frequency)
#        while not self.hmi.automode and not rospy.is_shutdown():
        while not rospy.is_shutdown():
            if not self.hmi.automode :
                # Publish topics
                self.hmi.publishDeadman()
                self.hmi.publishFeedback()
                self.hmi.publishCmdVel()
                # Spin
                self.r.sleep()
            else :
                return  'enterAutomode'
        return 'preempted'
        
class interfaceState(smach.State):
    """
        Implements HMI as smach state to be used in concurrence with autonomous behaviours
        Usage example:
            smach.Concurrence.add('HMI', interfaceState(self.hmi))
    """
    def __init__(self,hmi):
        smach.State.__init__(self, outcomes=['preempted'])
        # Instantiate HMI
        self.hmi = hmi
        self.r = rospy.Rate(self.hmi.publish_frequency)
        
    def execute(self, userdata):
        while not rospy.is_shutdown():         
            if self.hmi.automode :
                # Publish topics
                self.hmi.publishDeadman()
                self.hmi.publishFeedback()
                self.r.sleep()
            else :
                break
        return 'preempted'