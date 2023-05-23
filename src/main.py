# sysv_ipc is needed to access the shared memory where the camera image is present.
import sysv_ipc
# numpy and cv2 are needed to access, modify, or display the pixels
import numpy
import cv2
# OD4Session is needed to send and receive messages
import OD4Session
# Import the OpenDLV Standard Message Set.
import opendlv_standard_message_set_v0_9_10_pb2
# Import the other python files for functions
from cone_detection import get_color_mask, cone_contours
from steering_angle import steering_angle
from boundingbox import draw_bounding_boxes

#parsing arguments
import argparse

# Define the argument parser
parser = argparse.ArgumentParser(description='For custom modes of the script running.')
parser.add_argument('--verbose', action='store_true', help='print verbose output')
parser.add_argument('--cid', type=int, help='input custom cid number')
args = parser.parse_args()

global sampleTimeStamp

################################################################################
# This dictionary contains all distance values to be filled by function onDistance(...).
distances = { "front": 0.0, "left": 0.0, "right": 0.0, "rear": 0.0 };
cid = args.cid if args.cid else 112
################################################################################
# This callback is triggered whenever there is a new distance reading coming in.
def onDistance(msg, senderStamp, timeStamps):
    sampleTimeStamp = timeStamps[2]


# Create a session to send and receive messages from a running OD4Session;
# Replay mode: CID = 253
# Live mode: CID = 112
# TODO: Change to CID 112 when this program is used on Kiwi.
session = OD4Session.OD4Session(cid=cid)
# Register a handler for a message; the following example is listening
# for messageID 1039 which represents opendlv.proxy.DistanceReading.
# Cf. here: https://github.com/chalmers-revere/opendlv.standard-message-set/blob/master/opendlv.odvd#L113-L115
messageIDDistanceReading = 1039
session.registerMessageCallback(messageIDDistanceReading, onDistance, opendlv_standard_message_set_v0_9_10_pb2.opendlv_proxy_DistanceReading)
# Connect to the network session.
session.connect()

################################################################################
# The following lines connect to the camera frame that resides in shared memory.
# This name must match with the name used in the h264-decoder-viewer.yml file.
name = "/tmp/video0.argb"
# Obtain the keys for the shared memory and semaphores.
keySharedMemory = sysv_ipc.ftok(name, 1, True)
keySemMutex = sysv_ipc.ftok(name, 2, True)
keySemCondition = sysv_ipc.ftok(name, 3, True)
# Instantiate the SharedMemory and Semaphore objects.
shm = sysv_ipc.SharedMemory(keySharedMemory)
mutex = sysv_ipc.Semaphore(keySemCondition)
cond = sysv_ipc.Semaphore(keySemCondition)

################################################################################
# Main loop to process the next image frame coming in.
while True:
    # Wait for next notification.
    cond.Z()

    # Lock access to shared memory.
    mutex.acquire()
    # Attach to shared memory.
    shm.attach()
    # Read shared memory into own buffer.
    buf = shm.read()
    # Detach to shared memory.
    shm.detach()
    # Unlock access to shared memory.
    mutex.release()

    # Turn buf into img array (1280 * 720 * 4 bytes (ARGB)) to be used with OpenCV.
    img = numpy.frombuffer(buf, numpy.uint8).reshape(720, 1280, 4)
    if img.shape[2] == 4:
        img = cv2.cvtColor(img, cv2.COLOR_RGBA2RGB)

    ############################################################################
    blue_mask, yellow_mask = get_color_mask(img)
    blue_contours = cone_contours(blue_mask)
    yellow_contours = cone_contours(yellow_mask)
    

    # Draw blue bounding boxes for blue contours
    img = draw_bounding_boxes(img, blue_contours, color=(255, 0, 0)) # Blue color in BGR

    # Draw yellow bounding boxes for yellow contours
    img = draw_bounding_boxes(img, yellow_contours, color=(0, 255, 255)) # Yellow color in BGR
 


    # TODO: Disable the following two lines before running on Kiwi:
    cv2.imshow("image", img);
    cv2.waitKey(2);

    angleReading= steering_angle(blue_contours, yellow_contours)
    print ("group 16; " + str(angleReading) + " ; " + str(sampleTimeStamp) )
