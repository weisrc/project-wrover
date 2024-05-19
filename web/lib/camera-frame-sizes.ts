export enum CameraFrameSize {
  FRAMESIZE_96X96,    // 96x96
  FRAMESIZE_QQVGA,    // 160x120
  FRAMESIZE_QCIF,     // 176x144
  FRAMESIZE_HQVGA,    // 240x176
  FRAMESIZE_240X240,  // 240x240
  FRAMESIZE_QVGA,     // 320x240
  FRAMESIZE_CIF,      // 400x296
  FRAMESIZE_HVGA,     // 480x320
  FRAMESIZE_VGA,      // 640x480
  FRAMESIZE_SVGA,     // 800x600
  FRAMESIZE_XGA,      // 1024x768
  FRAMESIZE_HD,       // 1280x720
  FRAMESIZE_SXGA,     // 1280x1024
  FRAMESIZE_UXGA,     // 1600x1200
  // 3MP Sensors
  FRAMESIZE_FHD,      // 1920x1080
  FRAMESIZE_P_HD,     //  720x1280
  FRAMESIZE_P_3MP,    //  864x1536
  FRAMESIZE_QXGA,     // 2048x1536
  // 5MP Sensors
  FRAMESIZE_QHD,      // 2560x1440
  FRAMESIZE_WQXGA,    // 2560x1600
  FRAMESIZE_P_FHD,    // 1080x1920
  FRAMESIZE_QSXGA,    // 2560x1920
  FRAMESIZE_INVALID
};

export const CAMERA_FRAME_SIZES = [
  // { value: CameraFrameSize.FRAMESIZE_96X96, label: "96x96" },
  { value: CameraFrameSize.FRAMESIZE_QQVGA, label: "160x120" },
  { value: CameraFrameSize.FRAMESIZE_QCIF, label: "176x144" },
  { value: CameraFrameSize.FRAMESIZE_HQVGA, label: "240x176" },
  { value: CameraFrameSize.FRAMESIZE_240X240, label: "240x240" },
  { value: CameraFrameSize.FRAMESIZE_QVGA, label: "320x240" },
  { value: CameraFrameSize.FRAMESIZE_CIF, label: "400x296" },
  { value: CameraFrameSize.FRAMESIZE_HVGA, label: "480x320" },
  { value: CameraFrameSize.FRAMESIZE_VGA, label: "640x480" },
  { value: CameraFrameSize.FRAMESIZE_SVGA, label: "800x600" },
  // { value: CameraFrameSize.FRAMESIZE_XGA, label: "1024x768" },
  // { value: CameraFrameSize.FRAMESIZE_HD, label: "1280x720" },
  // { value: CameraFrameSize.FRAMESIZE_SXGA, label: "1280x1024" },
  // { value: CameraFrameSize.FRAMESIZE_UXGA, label: "1600x1200" },
  // { value: CameraFrameSize.FRAMESIZE_FHD, label: "1920x1080" },
  // { value: CameraFrameSize.FRAMESIZE_P_HD, label: "720x1280" },
  // { value: CameraFrameSize.FRAMESIZE_P_3MP, label: "864x1536" },
  // { value: CameraFrameSize.FRAMESIZE_QXGA, label: "2048x1536" },
  // { value: CameraFrameSize.FRAMESIZE_QHD, label: "2560x1440" },
  // { value: CameraFrameSize.FRAMESIZE_WQXGA, label: "2560x1600" },
  // { value: CameraFrameSize.FRAMESIZE_P_FHD, label: "1080x1920" },
  // { value: CameraFrameSize.FRAMESIZE_QSXGA, label: "2560x1920" },
  // { value: CameraFrameSize.FRAMESIZE_INVALID, label: "Invalid" },
];