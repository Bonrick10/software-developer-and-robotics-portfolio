import React from 'react';
import ReactPlayer from 'react-player';

export function RenderVideo ({
  video,
  i,
  setTargetObject,
  lastClickTime,
  setLastClickTime,
  setMenuPosition,
  setVideoIndex,
  setEditVideo,
  setIsVideoContextMenuVisible
}) {
  const handleVideoDoubleClick = () => {
    const now = Date.now();
    if (now - lastClickTime < 500) {
      setVideoIndex(i);
      setEditVideo(true);
      setTargetObject(video);
    } else {
      setLastClickTime(now);
      setEditVideo(false);
    }
  };

  const handleVideoContextMenu = (event) => {
    event.preventDefault();
    setIsVideoContextMenuVisible(true);
    setVideoIndex(i);
    setMenuPosition({ x: event.clientX, y: event.clientY });
  };

  return (
    <div
      key={i}
      className ='videoBox'
      onContextMenu={(e) =>
        handleVideoContextMenu(e)
      }
      onClick={() =>
        handleVideoDoubleClick()
      }
      style={{
        width: `${video.width}%`,
        height: `${video.height}%`,
        zIndex: `${video.zIndex}`
      }}>
      <ReactPlayer
        url={video.URL}
        playing={video.autoPlay}
        style={{
          width: '100%',
          height: '100%'
        }}
      />
    </div>
  )
}
