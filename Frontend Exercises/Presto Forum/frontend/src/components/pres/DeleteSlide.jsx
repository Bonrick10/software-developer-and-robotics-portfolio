import React from 'react';
import axios from 'axios';
import { Button } from 'react-bootstrap';

import { AuthContext, StoreContext } from '../../contexts';
// TODO: Make this a modal
export function DeleteSlide ({ presId, activeIndex, setSlides }) {
  const { storeData } = React.useContext(StoreContext);
  const { config } = React.useContext(AuthContext);

  const deleteSlide = async () => {
    try {
      const response = await axios.get('http://localhost:5005/store', config);
      const userStore = response.data.store;
      const presIndex = userStore.store.presentations.findIndex((currPres) => {
        return (currPres.presId.toString() === presId)
      });
      if (userStore.store.presentations[presIndex].slides.length > 1) {
        userStore.store.presentations[presIndex].slides.splice(activeIndex, 1)
        for (let i = activeIndex; i < userStore.store.presentations[presIndex].slides.length; i++) {
          userStore.store.presentations[presIndex].slides[i].slideCount--
        }
      } else {
        alert('Please delete presentation instead');
      }
      storeData(userStore);
      setSlides(userStore.store.presentations[presIndex].slides)
    } catch (err) {
      alert(err.response);
    }
  }
  return (
    <>
    <Button variant="danger" onClick={deleteSlide}>Delete Slide</Button>
    </>
  )
}
