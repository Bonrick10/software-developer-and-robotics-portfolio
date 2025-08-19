import React from 'react';
import queryString from 'query-string';
import { Swiper, SwiperSlide } from 'swiper/react';
import { Navigation, Pagination, A11y, Keyboard } from 'swiper/modules';
import ReactPlayer from 'react-player'
import { Navigate } from 'react-router-dom';
import { PreviewLogoutButton } from '../components/PreviewLogoutButton';
import { StoreProvider } from '../contexts/StoreContext';
import 'swiper/css';
import 'swiper/css/navigation';
import 'swiper/css/pagination';
import 'swiper/swiper-bundle.css';
import 'swiper/css/keyboard';

export function Preview () {
  const [slides, setSlides] = React.useState([]);
  const [token, setLocalToken] = React.useState('a');
  // setLocalToken(localStorage.getItem('token'));
  React.useEffect(() => {
    setLocalToken(localStorage.getItem('token'))
  }, []);
  console.log(token);
  React.useEffect(() => {
    const queryParams = queryString.parse(window.location.search);
    const arrayString = queryParams.data;
    const parsedArray = JSON.parse(arrayString);
    setSlides(parsedArray);
  }, []);
  console.log(slides)
  const [activeIndex, setActiveIndex] = React.useState(0);

  const handleSlideChange = (swiper) => {
    setActiveIndex(swiper.activeIndex);
    console.log(swiper.activeIndex);
  };
  if (token === 'null') {
    // Ensures localStorage token is null when reaching login route.
    localStorage.setItem('token', 'null');
    return <Navigate to ="/login" />
  }

  return (
    <>
    <>
    <StoreProvider>
      <PreviewLogoutButton setLocalToken={setLocalToken}/>
    </StoreProvider>
    <Swiper
      modules={[Navigation, Pagination, A11y, Keyboard]}
      paginationn={{ clickable: false }}
      navigation={{ prevEl: '.swiper-button-prev', nextEl: '.swiper-button-next' }}
      keyboard={{ enabled: true }}
      allowTouchMove={false}
      onSlideChange={(swiper) => handleSlideChange(swiper)}
      spaceBetween={0}
      slidesPerView={1}
    >
      <div className="swiper-button-prev" style={{ display: activeIndex === 0 ? 'none' : 'block' }}/>
      <div className="swiper-button-next" style={{ display: activeIndex === slides.length - 1 ? 'none' : 'block' }}/>
      {slides.map((slide, index) => (
        <SwiperSlide key={index}>
          <div className='previewSlide' style={{ background: slide.colour ? `${slide.backgroundColour}` : `linear-gradient(${slide.gradientDirection}, ${slide.gradientColourOne}, ${slide.gradientColourTwo})` }}> {slide.title}
            <div className='slideNum'>{slide.slideCount}</div>
            {slide.text.map((text, i) => (
              <div key={i} className='textBox' style={{ width: `${text.width}%`, height: `${text.height}%`, fontSize: `${text.fontSize}em`, color: `${text.textColour}`, fontFamily: `${text.fontFamily}`, zIndex: `${text.zIndex}` }}>
                <div style={{ overflow: 'scroll', width: '100%', height: '100%' }}> {text.text} </div>
              </div>
            ))}
            {slide.video.map((video, i) => (
              <div key={i} className ='videoBox' style={{ width: `${video.width}%`, height: `${video.height}%`, zIndex: `${video.zIndex}` }}>
                <ReactPlayer url={video.URL} playing={video.autoPlay} width='100%' height='100%'/>
              </div>
            ))}
            {slide.image.map((image, i) => (
              <div key={i} className ='imageBox' style={{ width: `${image.dimX}%`, height: `${image.dimY}%`, zIndex: `${image.zIndex}` }}>
                <img src={image.src} alt={image.alt} style={{ width: '100%', height: '100%' }}/>
              </div>
            ))}
          </div>
        </SwiperSlide>
      ))}
    </Swiper>
    </>
    </>
  );
}
