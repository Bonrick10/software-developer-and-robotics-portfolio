import React from 'react';
import axios from 'axios';
import { Button } from 'react-bootstrap';

import { AuthContext, StoreContext } from '../../contexts';

export function AddSlideButton ({ presId, setSlides }) {
  const { config } = React.useContext(AuthContext);
  const { storeData } = React.useContext(StoreContext);

  const createSlide = async () => {
    try {
      const response = await axios.get('http://localhost:5005/store', config);
      const userStore = response.data.store;
      const userPres = userStore.store.presentations;
      const presIndex = userPres.findIndex((currPres) => {
        return (currPres.presId.toString() === presId)
      });
      const currPres = userPres[presIndex];
      const newSlide = {
        title: 'Title Text',
        slideId: currPres.slideCount,
        backgroundColour: '',
        gradientDirection: '',
        gradientColourOne: '',
        gradientColourTwo: '',
        colour: true,
        gradient: true,
        text: [],
        video: [],
        image: [],
        code: [],
      };
      currPres.slideCount++;

      currPres.slides.push(newSlide);
      storeData(userStore);
      setSlides(currPres.slides)
      setTimeout(async () => {
        const response = await axios.get('http://localhost:5005/store', config);
        const userStore = response.data.store;
        const currPres = userStore.store.presentations.find((currPres) => {
          return (currPres.presId.toString() === presId)
        });
        const newSaveState = {
          time: Date.now(),
          slides: currPres.slides
        }
        console.log('timeout active');
        currPres.saveStates.push(newSaveState);
        storeData(userStore);
        console.log(userStore);
      }, 60000);
    } catch (err) {
      alert(err.response);
    }
  }
  return (
    <Button variant="primary" onClick={createSlide}>Add new slide</Button>
  );
}
