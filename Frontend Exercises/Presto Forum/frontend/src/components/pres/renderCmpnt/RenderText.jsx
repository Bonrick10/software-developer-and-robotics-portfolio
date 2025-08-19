import React from 'react';

export function RenderText ({
  text,
  i,
  setTargetObject,
  lastClickTime,
  setLastClickTime,
  setMenuPosition,
  setTextIndex,
  setEditText,
  setIsTextContextMenuVisible
}) {
  const handleTextContextMenu = (event) => {
    event.preventDefault();
    setIsTextContextMenuVisible(true);
    setTextIndex(i);
    setMenuPosition({ x: event.clientX * 1.2, y: event.clientY * 1.2 });
  };

  const handleTextDoubleClick = () => {
    const now = Date.now();
    if (now - lastClickTime < 500) {
      setTextIndex(i);
      setEditText(true);
      setTargetObject(text);
    } else {
      setLastClickTime(now);
      setEditText(false);
    }
  };

  return (
    <div
      key={i}
      className='textBox'
      onContextMenu={(e) =>
        handleTextContextMenu(e)
      }
      onClick={() =>
        handleTextDoubleClick()
      }
      style={{
        fontSize: `${text.fontSize}em`,
        color: `${text.textColour}`,
        zIndex: `${text.zIndex}`,
        width: '100%',
        height: '100%'
      }}>
      {text.text}
    </div>
  )
}
