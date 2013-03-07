#/****************************************************************************
# FroboMind wait_state.py
# Copyright (c) 2011-2013, author Leon Bonde Larsen <leon@bondelarsen.dk>
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#	* Redistributions of source code must retain the above copyright
#  	notice, this list of conditions and the following disclaimer.
#	* Redistributions in binary form must reproduce the above copyright
#  	notice, this list of conditions and the following disclaimer in the
#  	documentation and/or other materials provided with the distribution.
#	* Neither the name FroboMind nor the
#  	names of its contributors may be used to endorse or promote products
#  	derived from this software without specific prior written permission.
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
from smach import State

class WaitState(State):
    """
    Simple wait state that sleeps for a specified duration and then succeeds.
    @param duration: seconds to wait
    @return: 'preempted' if preempted and 'succeeded' if duration ends 
"""
    def __init__(self,duration):
        State.__init__(self,outcomes=['succeeded','preempted'])
        self.duration = duration
        self.rate = rospy.Rate(5)
        self.exit_time = rospy.Time.now()       
        
    def execute(self,userdata):
        self.exit_time = rospy.Time.now() + rospy.Duration(self.duration)
        while not rospy.is_shutdown() :
            if self.preempt_requested() :
                break
            elif rospy.Time.now() > self.exit_time :
                break
        if self.preempt_requested() :
            self.recall_preempt()
            return 'preempted'            
        else:
            return "succeeded"
    
