import React from 'react';
import { Button } from 'react-bootstrap';
import { Link } from 'react-router-dom';

export function RearrangeSlideButton ({ presId }) {
  return (
    <>
    <Link to={`/rearrange/${presId}`} style={{ textDecoration: 'none' }}>
      <Button variant="secondary">Rearrange</Button>
    </Link>
    </>
  );
}
