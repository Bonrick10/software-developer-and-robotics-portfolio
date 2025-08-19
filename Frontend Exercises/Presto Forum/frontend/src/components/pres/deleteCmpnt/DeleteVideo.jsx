import React from 'react';
import axios from 'axios';
import { Button } from 'react-bootstrap';

import { AuthContext, StoreContext } from '../../../contexts';

export function DeleteVideo ({ presId, activeIndex, videoIndex, setSlides }) {
  const { storeData } = React.useContext(StoreContext);
  const { config } = React.useContext(AuthContext);

  const deleteVideo = async () => {
    try {
      const response = await axios.get('http://localhost:5005/store', config);
      const userStore = response.data.store;
      const presIndex = userStore.store.presentations.findIndex((currPres) => {
        return (currPres.presId.toString() === presId)
      });
      userStore.store.presentations[presIndex].slides[activeIndex].video.splice(videoIndex, 1)
      storeData(userStore);
      setSlides(userStore.store.presentations[presIndex].slides);
      console.log(userStore.store.presentations[presIndex].slides[activeIndex].video);
    } catch (err) {
      alert(err.response);
    }
  }
  return (
    <>
    <Button variant="danger" onClick={deleteVideo}>Delete Video</Button>
    </>
  )
}
