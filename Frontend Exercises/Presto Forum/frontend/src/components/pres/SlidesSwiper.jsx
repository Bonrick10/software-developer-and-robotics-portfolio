import React from 'react';
import axios from 'axios';
import { useNavigate } from 'react-router-dom';
import { Swiper, SwiperSlide } from 'swiper/react';
import { Navigation, Pagination, A11y, Keyboard } from 'swiper/modules';
import 'swiper/css';
import 'swiper/css/navigation';
import 'swiper/css/pagination';
import 'swiper/swiper-bundle.css';
import 'swiper/css/keyboard';

import { AddCmpntGroup } from './addCmpnt';
import { EditTextBox, EditImageBox, EditVideoBox, EditCodeBox } from './editCmpnt';
import { DeleteText, DeleteImage, DeleteVideo, DeleteCode } from './deleteCmpnt';
import { DeleteSlide } from '.';
import { RenderSlide } from './renderCmpnt';
import { AddBackgroundTheme } from './AddBackgroundTheme';

import { AuthContext, StoreContext } from '../../contexts';

export function SlidesSwiper ({
  slides,
  slideIndex,
  setSlides,
  presId,
  cornerBox,
  setCornerBox,
  editText,
  setEditText,
  editVideo,
  setEditVideo,
  editImage,
  setEditImage,
  editCode,
  setEditCode,
  isTextContextMenuVisible,
  setIsTextContextMenuVisible,
  isVideoContextMenuVisible,
  setIsVideoContextMenuVisible,
  isImageContextMenuVisible,
  setIsImageContextMenuVisible,
  isCodeContextMenuVisible,
  setIsCodeContextMenuVisible
}) {
  const { config } = React.useContext(AuthContext);
  const { storeData } = React.useContext(StoreContext);

  const [lastClickTime, setLastClickTime] = React.useState(0);
  const [menuPosition, setMenuPosition] = React.useState({ x: 0, y: 0 });
  const [targetObject, setTargetObject] = React.useState({});

  const [textIndex, setTextIndex] = React.useState(0);
  const [videoIndex, setVideoIndex] = React.useState(0);
  const [imageIndex, setImageIndex] = React.useState(0);
  const [codeIndex, setCodeIndex] = React.useState(0);

  const [position, setPosition] = React.useState({ x: 10, y: 10 });
  const [dragging, setDragging] = React.useState(false);
  const [offset, setOffset] = React.useState({ x: 0, y: 0 });

  const handleMouseDown = (e) => {
    if (cornerBox === true) {
      setDragging(true);
      setOffset({
        x: e.clientX - position.x,
        y: e.clientY - position.y
      });
    }
  };

  const handleMouseUp = () => {
    if (cornerBox === true) {
      setDragging(false);
    }
  };

  const handleMouseMove = (e) => {
    if (cornerBox === true) {
      if (dragging) {
        setPosition({
          x: e.clientX - offset.x,
          y: e.clientY - offset.y
        });
      }
    }
  };

  const saveSlides = (newSlides) => {
    setSlides(newSlides);
    console.log('setting timeout');
    setTimeout(async () => {
      const response = await axios.get('http://localhost:5005/store', config);
      const userStore = response.data.store;
      const currPres = userStore.store.presentations.find((currPres) => {
        return (currPres.presId.toString() === presId)
      });
      const newSaveState = {
        time: Date.now(),
        slides: slides
      }
      console.log('timeout active');
      currPres.saveStates.push(newSaveState);
      storeData(userStore);
      console.log(userStore);
    }, 60000);
  }

  const navigate = useNavigate();
  const handleSlideChange = (swiper) => {
    navigate(`/editPres/${presId}/slide/${swiper.activeIndex}`);

    setEditText(false);
    setEditVideo(false);
    setEditImage(false);
    setEditCode(false);

    setIsTextContextMenuVisible(false);
    setIsVideoContextMenuVisible(false);
    setIsImageContextMenuVisible(false);
    setIsCodeContextMenuVisible(false);
  };

  const handleMenuClick = () => {
    setIsTextContextMenuVisible(false);
    setIsVideoContextMenuVisible(false);
    setIsImageContextMenuVisible(false);
    setIsCodeContextMenuVisible(false);
  };
  return (
    <>
    {isTextContextMenuVisible && (
      <div className="textDeleteContextMenu" style={{ left: menuPosition.x, top: menuPosition.y }} onClick={handleMenuClick} >
        <DeleteText presId={presId} activeIndex={slideIndex} textIndex={textIndex} setSlides={saveSlides} />
      </div>
    )}
    {isVideoContextMenuVisible && (
      <div className="videoDeleteContextMenu" style={{ left: menuPosition.x, top: menuPosition.y }} onClick={handleMenuClick} >
        <DeleteVideo presId={presId} activeIndex={slideIndex} videoIndex={videoIndex} setSlides={saveSlides} />
      </div>
    )}
    {isImageContextMenuVisible && (
      <div className="imageDeleteContextMenu" style={{ left: menuPosition.x, top: menuPosition.y }} onClick={handleMenuClick} >
        <DeleteImage presId={presId} activeIndex={slideIndex} imageIndex={imageIndex} setSlides={saveSlides} />
      </div>
    )}
    {isCodeContextMenuVisible && (
      <div className="codeDeleteContextMenu" style={{ left: menuPosition.x, top: menuPosition.y }} onClick={handleMenuClick} >
        <DeleteCode presId={presId} activeIndex={slideIndex} codeIndex={codeIndex} setSlides={saveSlides} />
      </div>
    )}
    <AddBackgroundTheme presId={presId} setSlides={saveSlides} activeIndex={slideIndex}/>
    <EditTextBox activeIndex={slideIndex} setSlides={saveSlides} presId={presId} textTarget={targetObject} textIndex={textIndex} editText={editText} setEditText={setEditText}/>
    <EditVideoBox activeIndex={slideIndex} setSlides={saveSlides} presId={presId} videoTarget={targetObject} videoIndex={videoIndex} editVideo={editVideo} setEditVideo={setEditVideo}/>
    <EditImageBox activeIndex={slideIndex} setSlides={saveSlides} presId={presId} imageTarget={targetObject} imageIndex={imageIndex} editImage={editImage} setEditImage={setEditImage}/>
    <EditCodeBox activeIndex={slideIndex} setSlides={saveSlides} presId={presId} codeTarget={targetObject} codeIndex={codeIndex} editCode={editCode} setEditCode={setEditCode}/>
    <Swiper
      modules={[Navigation, Pagination, A11y, Keyboard]}
      paginationn={{ clickable: true }}
      navigation={{ prevEl: '.swiper-button-prev', nextEl: '.swiper-button-next' }}
      keyboard={{ enabled: true }}
      allowTouchMove={false}
      onSlideChange={(swiper) => handleSlideChange(swiper)}
      spaceBetween={0}
      slidesPerView={1}
    >
      <div className="swiper-button-prev" style={{ display: parseInt(slideIndex) === 0 ? 'none' : 'block' }}/>
      <div className="swiper-button-next" style={{ display: parseInt(slideIndex) === slides.length - 1 ? 'none' : 'block' }}/>
      {slides.map((slide, index) => (
        <SwiperSlide key={index}>
          <RenderSlide
            slide={slide}
            presId={presId}
            activeIndex={index}
            lastClickTime={lastClickTime}
            setTargetObject={setTargetObject}
            setLastClickTime={setLastClickTime}
            setMenuPosition={setMenuPosition}
            setCornerBox={setCornerBox}
            onPointerDown={handleMouseDown}
            onPointerMove={handleMouseMove}
            onPointerUp={handleMouseUp}
            setSlides={saveSlides}
            setTextIndex={setTextIndex}
            setEditText={setEditText}
            setIsTextContextMenuVisible={setIsTextContextMenuVisible}
            setCodeIndex={setCodeIndex}
            setEditCode={setEditCode}
            setIsCodeContextMenuVisible={setIsCodeContextMenuVisible}
            setImageIndex={setImageIndex}
            setEditImage={setEditImage}
            setIsImageContextMenuVisible={setIsImageContextMenuVisible}
            setVideoIndex={setVideoIndex}
            setEditVideo={setEditVideo}
            setIsVideoContextMenuVisible={setIsVideoContextMenuVisible}
          ></RenderSlide>
        </SwiperSlide>
      ))}
    </Swiper>
    <DeleteSlide activeIndex={slideIndex} setSlides={saveSlides} presId={presId} />
    <AddCmpntGroup activeIndex={slideIndex} setSlides={saveSlides} presId={presId} />
    </>
  );
}
