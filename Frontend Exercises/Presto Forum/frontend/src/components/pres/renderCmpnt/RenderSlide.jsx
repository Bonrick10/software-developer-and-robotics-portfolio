import React from 'react';

import { RenderText, RenderImage, RenderVideo, RenderCode } from '.';

export function RenderSlide ({
  slide,
  activeIndex,
  setTargetObject,
  lastClickTime,
  setLastClickTime,
  setMenuPosition,
  // setCornerBox,
  // onPointerDown,
  // onPointerMove,
  // onPointerUp,
  setTextIndex,
  setEditText,
  setIsTextContextMenuVisible,
  setCodeIndex,
  setEditCode,
  setIsCodeContextMenuVisible,
  setImageIndex,
  setEditImage,
  setIsImageContextMenuVisible,
  setVideoIndex,
  setEditVideo,
  setIsVideoContextMenuVisible
}) {
  return (
    <div
      className='slide'
      style={{
        background: slide.colour ? `${slide.backgroundColour}` : `linear-gradient(${slide.gradientDirection}, ${slide.gradientColourOne}, ${slide.gradientColourTwo})`
      }}>
      <div className='slideNum'>{parseInt(activeIndex) + 1}</div>
      {slide.text.map((text, i) => (
        <div
          key={i}
          style={{
            position: 'absolute',
            width: `${text.width}%`,
            height: `${text.height}%`,
            top: `${text.top}%`,
            left: `${text.left}%`
          }}
        >
          <RenderText
            text={text}
            textIndex={i}
            setTargetObject={setTargetObject}
            lastClickTime={lastClickTime}
            setLastClickTime={setLastClickTime}
            setMenuPosition={setMenuPosition}
            setTextIndex={setTextIndex}
            setEditText={setEditText}
            setIsTextContextMenuVisible={setIsTextContextMenuVisible}
          ></RenderText>
        </div>
      ))}
      {slide.video.map((video, i) => (
        <RenderVideo
          video={video}
          key={i}
          videoIndex={i}
          setTargetObject={setTargetObject}
          lastClickTime={lastClickTime}
          setLastClickTime={setLastClickTime}
          setMenuPosition={setMenuPosition}
          setVideoIndex={setVideoIndex}
          setEditVideo={setEditVideo}
          setIsVideoContextMenuVisible={setIsVideoContextMenuVisible}
        ></RenderVideo>
      ))}
      {slide.image.map((image, i) => (
        <RenderImage
          image={image}
          key={i}
          imageIndex={i}
          setTargetObject={setTargetObject}
          lastClickTime={lastClickTime}
          setLastClickTime={setLastClickTime}
          setMenuPosition={setMenuPosition}
          setImageIndex={setImageIndex}
          setEditImage={setEditImage}
          setIsImageContextMenuVisible={setIsImageContextMenuVisible}
        ></RenderImage>
      ))}
      {slide.code.map((code, i) => (
        <RenderCode
          code={code}
          key={i}
          codeIndex={i}
          setTargetObject={setTargetObject}
          lastClickTime={lastClickTime}
          setLastClickTime={setLastClickTime}
          setMenuPosition={setMenuPosition}
          setCodeIndex={setCodeIndex}
          setEditCode={setEditCode}
          setIsCodeContextMenuVisible={setIsCodeContextMenuVisible}
        ></RenderCode>
      ))}
    </div>
  )
}
