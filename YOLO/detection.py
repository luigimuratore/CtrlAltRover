import cv2
from ultralytics import YOLO

# Load the YOLO model (change the weight file as needed, e.g., yolov8n.pt, yolov8s.pt, etc.)
model = YOLO("yolo11m.pt")

# Open a connection to your webcam (0 is usually the default camera)
cap = cv2.VideoCapture(0)

if not cap.isOpened():
    print("Error: Could not open webcam.")
    exit()

while True:
    # Capture frame-by-frame
    ret, frame = cap.read()
    if not ret:
        print("Failed to grab frame")
        break

    # Run YOLO inference on the current frame
    results = model(frame)

    # Get the annotated image (the first result in the list corresponds to the input frame)
    annotated_frame = results[0].plot()

    # Display the resulting frame
    cv2.imshow("YOLO Object Detection", annotated_frame)

    # Exit loop when 'q' is pressed
    if cv2.waitKey(1) & 0xFF == ord("q"):
        break

# When everything is done, release the capture and close windows
cap.release()
cv2.destroyAllWindows()
