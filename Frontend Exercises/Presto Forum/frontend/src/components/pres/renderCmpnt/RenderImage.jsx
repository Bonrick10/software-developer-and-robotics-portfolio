import React from 'react';
import { Image } from 'react-bootstrap';

export function RenderImage ({
  image,
  i,
  setTargetObject,
  lastClickTime,
  setLastClickTime,
  setMenuPosition,
  setImageIndex,
  setEditImage,
  setIsImageContextMenuVisible
}) {
  const handleImageDoubleClick = () => {
    const now = Date.now();
    if (now - lastClickTime < 500) {
      setImageIndex(i);
      setEditImage(true);
      setTargetObject(image);
    } else {
      setLastClickTime(now);
      setEditImage(false);
    }
  };

  const handleImageContextMenu = (event) => {
    event.preventDefault();
    setIsImageContextMenuVisible(true);
    setImageIndex(i);
    setMenuPosition({ x: event.clientX, y: event.clientY });
  };

  return (
    <div
      key={i}
      className ='imageBox'
      onContextMenu={(e) =>
        handleImageContextMenu(e)
      }
      onClick={() =>
        handleImageDoubleClick()
      }
      style={{
        width: `${image.width}%`,
        height: `${image.height}%`,
        zIndex: `${image.zIndex}`
      }}>
      <Image
        src={image.src}
        alt={image.alt}
        style={{
          width: '100%',
          height: '100%'
        }}
      />
    </div>
  )
}
