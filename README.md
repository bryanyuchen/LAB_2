LAB 2 Documentation

Bill Of Materials:-----------------------------------------------------------------------------------------------------------------------

Wiring Schematic:------------------------------------------------------------------------------------------------------------------------

Mechanical Drawing:----------------------------------------------------------------------------------------------------------------------

Code:------------------------------------------------------------------------------------------------------------------------------------
Please see associated Arduino file 

Design of System: -----------------------------------------------------------------------------------------------------------------------

Pre-test Inspiration:

The inspiration for this design comes from both my piano playing background as well as from Lab 1. Because I did not have time to complete lab 1, I was hoping to incorporate some of the concepts from Lab 1 into Lab 2. Thus, I came up with this design to utilize both forward/inverse kinematic and piano playing concepts to build a animatronic piano-playing system.

This system is a very basic kinematic linkage with only two joint and an end effector. Because the orientation of the end effector is not important, the necessary degrees of freedom are cut down to 3, making computation much simpler. The kinematic linkage can be observed in the section titled 'Mechanical Drawing'. As can be seen, the first servo-controlled joint serves to rotate the cardboard slab to move the second joint to a certain position on the keyboard, whereby the servo controlling the second joint can effectively press down on a piano key and 'play' a note.

The code for Forward Kinematics can be seen in the attached .m Matlab file. Because of this kinematic linkage's contrained range of motion, I thought that writing code for inverse kinematics is unecessary. The system is already set up (by default, the second servo positioned between the notes E and F, which 1) places the default position right in the middle of the give 6 note range and 2) provides a simple and accurate method to place the system each time for performance) such that its range of motion covers 6 notes: C,D,E,F,G,A, of the C major scale, therefore, all that is left to discern is the correct angle to position the second servo in the range of the correct note. Each note is measured to be approximately 2.4cm in diameter, which is the y axis in the world reference frame. Therefore, we only need to calculate joint angles to place us in the correct y-axis coordinates. This is done by taking the total transformation obtained from forward kinematics and extracting the equation for the y coordinates of the end effector: (27*sin(T1))/2 + 2*sin(T1)*sin(T2). 

Since we have two variables, we could do optimization to calculate parameters the minimize range of motion; however, I realized since the second servo's usage is more for playing the notes, we can just assume ignore the second variable T2 = 0 and get a unique solution solving y = (27*sin(T1))/2. 
TI = arcsin(2*y/27)

The following list contains the data for each of the piano notes:

C: 4.8cm:7.2cm -> 0.363508:0.562536 rad
 
D: 2.4cm:4.8cm -> 0.178728:0.363508 rad
 
E: 0+:2.4cm -> 0+:0.178728 rad
 
F: 0-:-2.4cm -> 0-:-0.178728 rad
 
G: -2.4cm:-4.8cm -> -0.178728:-0.363508 rad
 
A: -4.8cm:-7.2cm -> -0.363508:-0.562536 rad

Obviously, arduino and the servo cannot exact this kind of precision. Let us first convert to degrees (which is how arduino is programmed to actuate the servo) and then round off.

C: 20.8:32.2 degreees
 
D: 10.2:20.8 degrees
 
E: 0+:10.2 degrees 
 
F: 0-:-10.2 degrees
 
G: -10.2:-20.8 degrees
 
A: -20.8:-32.2 degrees

Then, we will pick a point within the degree boundaries that is easy for a human to understand:

C: 30 degrees
 
D: 15 degrees
 
E: 5 degrees
 
F: -5 degrees
 
G: -15 degrees
 
A: -30 degrees

The angular speed of the servo is listed as 0.09 sec/60°
This will be utilized in relation to a delay introduced to create the illusion of tempo in the code. In other words, the microcontroller will calculate the time it will take for servo 1 to reach the correct position based on this information, and then introduce a delay before activating servo 2 to play a note. The longest time it would take for this system is to travel from C to A, a total of 50 degrees, which is 0.075 sec. To play a note, we will need to rotate the second servo, which is the continuous servo from a position of -90 degrees to zero degrees and back roughly, which would take 0.27 sec (since this is a continuous servo, we can't set exact angles, so I used my best judgement on the delay time to set these angles - it seemed like the continuous servo was calibrated wrong, and when set to rotate first in the clockwise direction, then in the counterclockwise, with equal delay times, the servo would rotate more in counterclockwise than in the clockwise direction). The total time would thus be 0.345 sec. If this time period were to be used as tempo, it would translate into around 170 bpm which is unecessarily fast. Therefore, we will set the tempo to 120bpm, and implement delays based on this:

Tempo = 120 bpm
 
Period = 0.5 sec
 
Period - time it takes to play a note = 0.5 sec - 0.27 sec = 0.23 sec
 
Delay = 0.23 sec - time to travel from one note to the next


Post-Test Findings:
While theoretically, my design should be flawless, testing the system up to this point helped me to uncover a few real-world issues

ISSUE 1:
After working with the continuous servo as a normal servo, it became apparent that this was impossible, so I switched to a new strategy. I would embrace the continuous servo for what it is and let it continuously rotate, thereby modifying the delay times. I also realized that the delay times given in the specs are inacurrate, so I measured them myself:

continuous servo 360 degree rotation time = 0.8 sec
normal servo = 0.3 sec for 60 degrees

Therefore the new tempo will be 1 cycle /0.8 sec * 60 sec/min = 75 bpm
delay = 1.1 - travel time (The value 1.1 was achieved empirically through trial and error

Edit: After more error and issues came up, I just simplified my design and simply placed a delay(850-900), which seemed to work rather well since the note sequences I had the system play with mostly notes with similar consecutive distances. The value 850-900 would be adjusted according to this distance. I would set this value to 880 by default

ISSUE 2:
The angles written in arduino do not translate well into the actual angles the servo rotates by. For small angles such as 10 degrees, the servo often does not even react. Sometimes it recognizes this change and rotates by a little, but most of the time it doesn't, therefore, I moved the setup closer to the keyboard to allow for wider angles. In addition, I realized most negative angles don't work so I recalibrated my angles:

   'c' = 55 degrees
   
   'd' = 40  degrees

   'e' = 32  degrees

   'f' = 25  degrees

   'g'= 15  degrees

   'a' = 0  degrees
    
ISSUE 3:

It was often hard for servo 2 to press down on the keys to make a sound - I didn't realize how much force was required to play a note on a piano. To deal with this issue, I taped stacks of quarters down on each key, carefully weighed to provide enough weight on the key so that a gentle stroke would allow the key to be played. In addition, I attached some batteries on the arm of the system to provide more weight to servo 2.

ISSUE 4:

The connection between servo 1 and the carboard on which servo 2 sits turned out to be rather weak. Therefore, I screwed a screw into the cardboard to secure it with the tip of servo 1's arm. In addition, I stuffed both sides of servo 1's arm with scrunched paper to provide more support in its connection with the cardboard.

With these issues pointed out and fixes implemented, the results can be seen with the following video link:


To improve on this design in the future, I would replace the cardboard slabs with 3d printed materials, as the carboard's instability seemed to magnify errors. If possible, I would also search for a longer arm to attach onto servo 1, so that it can form a stronger connection with the cardboard slab or 3d printed slab. In addition, I would also replace the continuous servo 2 with a normal servo, so that instead of rotation 360 degrees, servo 2 could simply rotate downwards to play a key, and then come back up. This would first of all, be able to play notes faster than the continous arm servo, and secondly, help to simplify design of the slab, which I had to cut a whole in to account for the continuous rotation of the arm. If necessary, replacing servo 2 with a normal servo would also provide control into the extent to which servo 2 presses down on the notes, controlling the dynamics of the music, or in other words, whether to play the note loud or soft.
