import React from 'react';
import axios from 'axios';
import { useParams } from 'react-router-dom';

import { EditNavBar, AddSlideButton, SlidesSwiper, RearrangeSlideButton } from '../components/pres';
import { PreviewButton } from '../components/PreviewButton'
import { AuthContext, StoreProvider } from '../contexts';

export function Edit () {
  const { presId, slideIndex } = useParams();
  const { config } = React.useContext(AuthContext);
  const [slides, setSlides] = React.useState([]);
  const [editText, setEditText] = React.useState(false);
  const [editImage, setEditImage] = React.useState(false);
  const [editVideo, setEditVideo] = React.useState(false);
  const [editCode, setEditCode] = React.useState(false);

  const [isTextContextMenuVisible, setIsTextContextMenuVisible] = React.useState(false);
  const [isImageContextMenuVisible, setIsImageContextMenuVisible] = React.useState(false);
  const [cornerBox, setCornerBox] = React.useState(false);
  const [isVideoContextMenuVisible, setIsVideoContextMenuVisible] = React.useState(false);
  const [isCodeContextMenuVisible, setIsCodeContextMenuVisible] = React.useState(false);

  React.useEffect(() => {
    const getSlides = async () => {
      try {
        const response = await axios.get('http://localhost:5005/store', config);
        const userStore = response.data.store;
        const presIndex = userStore.store.presentations.findIndex((currPres) => {
          return (currPres.presId.toString() === presId)
        });
        setSlides(userStore.store.presentations[presIndex].slides);
        console.log(slides);
      } catch (err) {
        alert(err.response);
      }
    }
    getSlides();
  }, [])

  const [clickCount, setClickCount] = React.useState(0);
  const [lastClickTime, setLastClickTime] = React.useState(0);

  const handleClick = () => {
    const now = Date.now();
    if (now - lastClickTime < 500) {
      setClickCount(0);
    } else {
      setClickCount(clickCount + 1);
      setLastClickTime(now);

      setEditText(false);
      setEditVideo(false);
      setEditImage(false);
      setEditCode(false);

      setIsTextContextMenuVisible(false);
      setIsVideoContextMenuVisible(false);
      setIsImageContextMenuVisible(false);
      if (cornerBox === true) {
        setCornerBox(false);
      }
      setIsCodeContextMenuVisible(false);
    }
  };
  return (
    <div onClick={handleClick} onChange={handleClick}>
      <StoreProvider>
        <EditNavBar presId={presId}></EditNavBar>
        <AddSlideButton setSlides={setSlides} presId={presId}></AddSlideButton>
        <PreviewButton slides={slides}/>
        <RearrangeSlideButton presId={presId}/>
        <SlidesSwiper
          slides={slides}
          slideIndex={slideIndex}
          setSlides={setSlides}
          presId={presId}
          cornerBox={cornerBox}
          setCornerBox={setCornerBox}
          editText={editText}
          setEditText={setEditText}
          editVideo={editVideo}
          setEditVideo={setEditVideo}
          editImage={editImage}
          setEditImage={setEditImage}
          editCode={editCode}
          setEditCode={setEditCode}
          isTextContextMenuVisible={isTextContextMenuVisible}
          setIsTextContextMenuVisible={setIsTextContextMenuVisible}
          isVideoContextMenuVisible={isVideoContextMenuVisible}
          setIsVideoContextMenuVisible={setIsVideoContextMenuVisible}
          isImageContextMenuVisible={isImageContextMenuVisible}
          setIsImageContextMenuVisible={setIsImageContextMenuVisible}
          isCodeContextMenuVisible={isCodeContextMenuVisible}
          setIsCodeContextMenuVisible={setIsCodeContextMenuVisible}
          />
      </StoreProvider>
    </div>
  );
}
